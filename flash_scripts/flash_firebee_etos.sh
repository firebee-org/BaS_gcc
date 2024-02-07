#!/bin/bash

pushd ../../emutos
make clean firebee -j
m68k-elf-objcopy --alt-machine-code 4 -Isrec -Oelf32-m68k etosfbus.s19 emutosfb.elf
mv emutosfb.elf ../BaS_gcc/flash_scripts/emutos-firebee.elf
popd
time bdmctrl flash_firebee_etos.bdm /dev/bdmcf3
