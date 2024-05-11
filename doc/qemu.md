# qemu仿真riscv
目标是仿真riscv32平台机器

## 工具链

qemu-system: https://www.nucleisys.com/upload/files/toochain/qemu/nuclei-qemu-2022.04-linux-x64.tar.gz

交叉编译工具链： https://www.nucleisys.com/upload/files/toochain/gcc/nuclei_riscv_newlibc_prebuilt_linux64_2022.04.tar.bz2

- 下载上述两个软件包并传输至ubuntu中
- 在ubuntu中解压这两个软件包
- 将这两个包的路径添加到用户环境变量

## 仿真步骤
在ezos路径执行make qemu，展开执行的指令为```qemu-system-riscv32 -nographic -machine virt -net none \
-chardev stdio,id=con,mux=on -serial chardev:con \
-mon chardev=con,mode=readline -bios none \
-smp 1 -kernel /local/study/ezos/out/ezos.out -s```

具体含义：

- qemu-system-riscv32: 这是启动 QEMU 模拟器的命令

- nographic: 这个选项告诉 QEMU 不要使用图形界面，而是将控制台输出发送到标准输出，这在需要在终端中运行并观察输出时很有用。

- machine virt: 这个选项指定了使用虚拟的 "virt" 机器类型，这是一种支持虚拟化的通用机器模型。

- net none: 这个选项禁用了网络连接，意味着虚拟机不会连接到任何网络。

- chardev stdio,id=con,mux=on: 这个选项创建了一个名为 "con" 的字符设备，并将其与标准输入输出连接起来，这样就可以在终端中交互地与虚拟机通信。

- serial chardev:con: 这个选项将串行端口连接到之前创建的字符设备 "con"，这样虚拟机的串行输出就会被发送到标准输出，从而在终端中显示出来。

- mon chardev=con,mode=readline: 这个选项将 QEMU 的监视器连接到字符设备 "con"，并以交互模式（mode=readline）运行，这样就可以通过终端输入命令来与监视器进行交互。

- bios none: 这个选项指定不使用任何 BIOS，因为在这种情况下，启动的操作系统（或内核）通常已经包含了引导加载器（bootloader）或 BIOS。

- smp 1: 这个选项指定虚拟机只有一个 CPU 核心。

- kernel /local/study/ezos/out/ezos.out: 这个选项指定了要加载到虚拟机中的内核文件的路径，这个路径是 /local/study/ezos/out/ezos.out。

- 综上，使用 QEMU 模拟器启动一个 RISC-V 32 位架构的虚拟机，禁用网络连接，将控制台输出发送到标准输出，允许通过终端与虚拟机进行交互，并且加载指定的内核文件。

仿真riscv芯片信息：
- 1 PLIC
- 1 CLINT
- 1 NS16550
- memory：0x80000000, LENGTH = 1M

## gdb调试

step：
- 在ezos路径执行make qemu_gdb
- 打开另一个端口，执行riscv-nuclei-elf-gdb out/ezos.out
- 在gdb端口连接，执行tar rem:1234
- load & run
