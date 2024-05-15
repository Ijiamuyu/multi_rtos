# qemu仿真riscv
目标是仿真riscv32平台机器

## 工具链
兼容win or linux
### linux
qemu-system: https://www.nucleisys.com/upload/files/toochain/qemu/nuclei-qemu-2022.04-linux-x64.tar.gz

交叉编译工具链： https://www.nucleisys.com/upload/files/toochain/gcc/nuclei_riscv_newlibc_prebuilt_linux64_2022.04.tar.bz2

cmake > 3.12

- 下载上述两个软件包并传输至ubuntu中
- 在ubuntu中解压这两个软件包
- 将这两个包的路径添加到用户环境变量

### win

- gitbash:添加路径到vscode
- qemu:D:\NucleiStudio\toolchain\qemu\bin
- riscv-nuclei-elf-gcc:D:\NucleiStudio\toolchain\gcc\bin
- make & cmake:需支持MinGW
- 在gitbash端export工具链到~/.bashrc

## 仿真(win & linux)

- run qemu
  - ./build.sh build (win or linux)
  - ./build.sh run

- gdb
  - ./build.sh gdb
  - riscv-nuclei-elf-gdb out/ezos_app.out
  - tar rem:1234
  - load & run

