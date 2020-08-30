#!/bin/bash

pushd ../../emutos
make clean m548x-bas -j
m68k-elf-objcopy --alt-machine-code 4 -Isrec -Oelf32-m68k emutos-m548x-bas.s19 emutos-m548x-bas.elf
mv emutos-m548x-bas.elf ../BaS_gcc/flash_scripts/emutos-m548x-bas.elf
popd
time bdmctrl flash_m548x_etos.bdm /dev/tblcf4
