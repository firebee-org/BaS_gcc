# BaS_gcc
FireBee gcc-based firmware

This is BaS_gcc, an alternative Firmware for the FireBee.

What the heck is the FireBee? An ATARI Falcon clone using the ColdFire m5474 µC and an FPGA.
See here: http://firebee.org

The original FireBee distribution comes with "original" BaS - a minimal fimware coded in C and
assembler using the FreeScale CodeWarrior development environment. Original BaS implements the minimal
functionality to boot up FireTOS (which is a patched original Falcon TOS).

The FireBee can also run EmuTOS (see http://emutos.sourceforge.net),
a completely free and open source TOS.
Contrary to FireTOS (that contains an m68k emulation to emulate a "traditional" m68k chip), EmuTOS is available as native ColdFire implementation. 
The FireBee can also run FreeMiNT (a preemptive multitasking extension to TOS).

BaS_gcc aims to fully support native EmuTOS + native FreeMiNT on the FireBee.
