#!/bin/bash

CMD=$1
TARGET=$2
ARCH=riscv
PROJECT_PATH=${PWD}
PROJECT=ezos

function show_help()
{
    echo "./build.sh type(build | clean | run | gdb) [target(win linux)]"
}

if [[ "${CMD}" == "build" ]]; then
    if [[ "${TARGET}" != "win" && "${TARGET}" != "linux" ]]; then
        echo "Invalid project to build!"
        echo "Build failed!"
        show_help
        exit 1
    fi
    rm -rf ${PROJECT_PATH}/build
    rm -rf ${PROJECT_PATH}/out
    mkdir ${PROJECT_PATH}/build
    mkdir ${PROJECT_PATH}/out
    cd ${PROJECT_PATH}/build
    if [[ "${TARGET}" == "win" ]]; then
    echo "build target for windows..."
        cmake .. -G "MinGW Makefiles" -DBUILD_TARGET=${TARGET} -DTARGET_NAME=${PROJECT}_app
    fi
    if [[ "${TARGET}" == "linux" ]]; then
    echo "build target for linux..."
        cmake .. -G "Unix Makefiles"  -DBUILD_TARGET=${TARGET} -DTARGET_NAME=${PROJECT}_app
    fi
    make
    if [[ $? == 0 ]]; then
        cp ${PROJECT_PATH}/build/${PROJECT}_app.out ${PROJECT_PATH}/out
        cp ${PROJECT_PATH}/build/${PROJECT}_app.map ${PROJECT_PATH}/out
        echo "Build success!"
    else
        echo "Build failed!"
        show_help
        exit 1
    fi
    riscv-nuclei-elf-objdump -xdts ${PROJECT_PATH}/out/${PROJECT}_app.out > ${PROJECT_PATH}/out/${PROJECT}_app.s
fi

if [[ "${CMD}" == "clean" ]]; then
    rm -rf ${PROJECT_PATH}/build
    rm -rf ${PROJECT_PATH}/out
fi

if [[ "${CMD}" == "run" ]]; then
	qemu-system-riscv32 -nographic -machine virt -net none \
	-chardev stdio,id=con,mux=on -serial chardev:con \
	-mon chardev=con,mode=readline -bios none \
	-smp 1 -kernel ${PROJECT_PATH}/out/${PROJECT}_app.out
fi

if [[ "${CMD}" == "gdb" ]]; then
	qemu-system-riscv32 -nographic -machine virt -net none \
	-chardev stdio,id=con,mux=on -serial chardev:con \
	-mon chardev=con,mode=readline -bios none \
	-smp 1 -kernel ${PROJECT_PATH}/out/${PROJECT}_app.out -s -S
fi
