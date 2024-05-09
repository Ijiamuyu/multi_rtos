VERSION = 0.0.1
TARGET  = ezos
CURDIR  = $(shell pwd)
CROSS   = riscv-nuclei-elf-
LDFLAGS = -march=rv32imafdc -mabi=ilp32d -mcmodel=medany
CFLAGS  = -march=rv32imafdc -mabi=ilp32d -mcmodel=medany
ASFLAGS = -march=rv32imafdc -mabi=ilp32d -mcmodel=medany
CC      = $(CROSS)gcc
OBJCOPY = $(CROSS)objcopy
ARCH    = $(CROSS)ar
OBJDUMP = $(CROSS)objdump

KERNEL_DIR   = $(CURDIR)/kernel
APP_DIR      = $(CURDIR)/app
PORTABLE_DIR = $(CURDIR)/portable
INCLUDE_DIR  = $(CURDIR)/include
OUTPUT_DIR   = $(CURDIR)/out
DRIVER_DIR   = $(CURDIR)/driver
BUILD_DIR    = build

CPPFLAGS = \
	-D__riscv_float_abi_soft \
	-DportasmHANDLE_INTERRUPT=handle_trap \
	-I$(INCLUDE_DIR) \
	-I$(KERNEL_DIR)/include \
	-I$(DRIVER_DIR)/

CFLAGS  ?= -march=rv32ima -mabi=ilp32 -mcmodel=medany
CFLAGS	+= -Wall \
	-fmessage-length=0 \
	-ffunction-sections \
	-fdata-sections \
	-fno-builtin-printf \
	-Og -ggdb3


ASFLAGS ?= -march=rv32ima -mabi=ilp32 -mcmodel=medany
LDFLAGS += -nostartfiles -Tfake_rom.lds \
	-Xlinker --gc-sections \
	-Xlinker --defsym=__stack_size=300

KERNEL_C_SRCS = \
	$(KERNEL_DIR)/source/ez_core.c \
	$(KERNEL_DIR)/source/ez_task.c \
	$(KERNEL_DIR)/source/ez_var.c

PORTABLE_C_SRCS = \
	$(PORTABLE_DIR)/riscv-virt.c \
	$(PORTABLE_DIR)/ez_cpu.c

DRIVER_C_SRCS = \
	$(DRIVER_DIR)/ns16550.c

PORTABLE_ASM_SRCS = \
	$(PORTABLE_DIR)/start.S \
	$(PORTABLE_DIR)/portASM.S

C_SRCS   = $(APP_DIR)/main.c $(PORTABLE_C_SRCS) $(DRIVER_C_SRCS) $(KERNEL_C_SRCS)
ASM_SRCS = $(PORTABLE_ASM_SRCS)
SRCS     = $(C_SRCS) $(ASM_SRCS)


$(BUILD_DIR)/$(TARGET).out: $(SRCS) fake_rom.lds Makefile
	@make clean
	@mkdir -p $(OUTPUT_DIR) $(BUILD_DIR)
	$(CC) $(CFLAGS) $(ASFLAGS) $(CPPFLAGS) $(LDFLAGS) $(SRCS) -Wl,--Map=$(BUILD_DIR)/$(TARGET).map -o $@
	@cp $(BUILD_DIR)/$(TARGET).out $(OUTPUT_DIR)
	@cp $(BUILD_DIR)/$(TARGET).map $(OUTPUT_DIR)
	@$(OBJDUMP) -xdts $(OUTPUT_DIR)/$(TARGET).out > $(OUTPUT_DIR)/$(TARGET).s
	@make check


clean:
	@rm -rf $(OUTPUT_DIR)
	@rm -rf $(BUILD_DIR)

check:
	@if [ -e $(OUTPUT_DIR)/$(TARGET).out ]; then \
		echo "build $(TARGET).out successful"; \
		else \
			echo " build $(TARGET).out failed!"; \
	fi

start_qemu_gdb:
	qemu-system-riscv32 -nographic -machine virt -net none \
	-chardev stdio,id=con,mux=on -serial chardev:con \
	-mon chardev=con,mode=readline -bios none \
	-smp 1 -kernel $(OUTPUT_DIR)/$(TARGET).out -s -S

start_qemu:
	qemu-system-riscv32 -nographic -machine virt -net none \
	-chardev stdio,id=con,mux=on -serial chardev:con \
	-mon chardev=con,mode=readline -bios none \
	-smp 1 -kernel $(OUTPUT_DIR)/$(TARGET).out

qemu_gdb:
	@if [ -e $(OUTPUT_DIR)/$(TARGET).out ]; then \
		echo "start qemu..."; \
		make start_qemu_gdb \
	else \
		echo "cannot find $(TARGET).out!"; \
	fi

qemu:
	@if [ -e $(OUTPUT_DIR)/$(TARGET).out ]; then \
		echo "start qemu..."; \
		make start_qemu \
	else \
		echo "cannot find $(TARGET).out!"; \
	fi


.PHONY: check clean qemu_gdb start_qemu start_qemu_gdb qemu
