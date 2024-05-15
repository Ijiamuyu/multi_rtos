#!/bin/bash

QEMU_PATH=/d/NucleiStudio/toolchain/qemu/bin
GCC_CROSS_PATH=/d/NucleiStudio/toolchain/gcc/bin
MAKE_PATH=/d/software/mingw64/bin
CMAKE_PATH=/d/software/cmake/bin

echo "add toolchain path"
echo "${QEMU_PATH}"
echo "${GCC_CROSS_PATH}"
echo "${MAKE_PATH}"
echo "${CMAKE_PATH}"

export PATH=$PATH:${QEMU_PATH}:${GCC_CROSS_PATH}:${MAKE_PATH}:${CMAKE_PATH}
