#!/bin/bash

CMD=$1
ARCH=$2
RTOS=$3
PROJECT_PATH=${PWD}
PROJECT=multi_rtos

function show_help()
{
    echo "./build.sh type(build | clean | run | gdb) [arch(riscv | arm)] [rtos(threadx | freertos | rt_thread | ezos)]"
}

if [[ "${CMD}" == "build" ]]; then
    if [[ "${ARCH}" == "riscv"  ]]; then
        export CROSS_COMPILE=riscv64-unknown-elf-
    elif [[ "${ARCH}" == "arm"  ]]; then
        export CROSS_COMPILE=
    else 
        echo "Invalid project to build!"
        echo "Build failed!"
        show_help
        exit 1
    fi

    if [[ "${RTOS}" != "threadx" && "${RTOS}" != "freertos" && "${RTOS}" != "rt_thread" && "${RTOS}" != "ezos" ]]; then
        echo "Invalid project to build!"
        echo "Build failed!"
        show_help
        exit 1
    fi
    rm -rf ${PROJECT_PATH}/build
    rm -rf ${PROJECT_PATH}/out
    mkdir -p ${PROJECT_PATH}/build
    mkdir -p ${PROJECT_PATH}/out
    cd ${PROJECT_PATH}/build

    echo "build arch ${ARCH}, rtos ${RTOS}"
    cmake .. -G "Unix Makefiles"  -DBUILD_ARCH=${ARCH} -DRTOS=${RTOS} -DTARGET_NAME=${PROJECT}
    make
    if [[ $? == 0 ]]; then
        cp ${PROJECT_PATH}/build/${PROJECT}.out ${PROJECT_PATH}/out
        cp ${PROJECT_PATH}/build/${PROJECT}.map ${PROJECT_PATH}/out
        echo "Build success!"
    else
        echo "Build failed!"
        show_help
        exit 1
    fi
    ${CROSS_COMPILE}objdump -xdts ${PROJECT_PATH}/out/${PROJECT}.out > ${PROJECT_PATH}/out/${PROJECT}.s
fi

if [[ "${CMD}" == "clean" ]]; then
    rm -rf ${PROJECT_PATH}/build
    rm -rf ${PROJECT_PATH}/out
fi

if [[ "${CMD}" == "run" ]]; then
	qemu-system-riscv32 -nographic -machine virt -net none \
	-chardev stdio,id=con,mux=on -serial chardev:con \
	-mon chardev=con,mode=readline -bios none \
	-smp 1 -kernel ${PROJECT_PATH}/out/${PROJECT}.out
fi

if [[ "${CMD}" == "gdb" ]]; then
	qemu-system-riscv32 -nographic -machine virt -net none \
	-chardev stdio,id=con,mux=on -serial chardev:con \
	-mon chardev=con,mode=readline -bios none \
	-smp 1 -kernel ${PROJECT_PATH}/out/${PROJECT}.out -s -S
fi
