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
    make -j $(nproc)
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
    if [[ "${ARCH}" == "riscv"  ]]; then
        qemu-system-riscv32 -nographic -machine virt -net none \
        -chardev stdio,id=con,mux=on -serial chardev:con \
        -mon chardev=con,mode=readline -bios none \
        -smp 1 -kernel ${PROJECT_PATH}/out/${PROJECT}.out
    elif [[ "${ARCH}" == "arm"  ]]; then
        echo "Invalid Arch to run!"
        show_help
        exit 1
    else
        echo "Invalid Arch to run!"
        show_help
        exit 1
    fi
fi

if [[ "${CMD}" == "gdb" ]]; then
    echo "multi rtos gdb is runing..., please type tar rem:1234 to access"
    if [[ "${ARCH}" == "riscv"  ]]; then
        qemu-system-riscv32 -nographic -machine virt -net none \
        -chardev stdio,id=con,mux=on -serial chardev:con \
        -mon chardev=con,mode=readline -bios none \
        -smp 1 -kernel ${PROJECT_PATH}/out/${PROJECT}.out -s -S
    elif [[ "${ARCH}" == "arm"  ]]; then
        echo "Invalid Arch to debug!"
        show_help
        exit 1
    else
        echo "Invalid Arch to debug!"
        show_help
        exit 1
    fi
fi

if [[ "${CMD}" != "build" && "${CMD}" != "clean" && "${CMD}" != "run" && "${CMD}" != "gdb" ]]; then
    echo "Invalid CMD!"
    show_help
    exit 1
fi
