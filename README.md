# BaS_gcc
**FireBee gcc-based firmware**

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

BaS_gcc aims to fully supports native EmuTOS + native FreeMiNT on the FireBee.


## How to build:

- checkout a copy of the BaS_gcc sources to your local machine
- make sure you have a suitable gcc toolchain available. This could be either an m68k-atari-mint toolchain or an m68k-elf gcc toolchain (see switches in the toplevel Makefile).
- type "make". The build should finish without errors
- for the FireBee, you'll find a flashable file (bas.s19) in the firebee directory
- for the m5484lite board files will reside in the m5475evb directory
- for the m54455 FreeScale evaluation board, files (not functional yet) are in the respective directory

## How to flash:

These files can be flashed with either Didier Méquinon's flasher available from the firebee.org home page or any BDM hardware (urgently recommended as this is the only "parachute" available if something goes wrong during flashing). Personally, I'm using a parallel P&E flash solution that came with my m5484lite board and the BDM tools from here: https://sourceforge.net/projects/bdm/ since they are supported under Linux (my favorite) and also support the TBLCF (another, USB based open source BDM solution). See the firebee.org home page for more information.

# ALL-CLEAR
The warning I had here before is superfluous. The problem has been fixed with R_0_9_4.
That means you can now savely flash BaS_gcc from EmuTOS using `FLASH_CF.PRG`

## What do I gain using BaS_gcc compared to "original" BaS?

From a pure user's perspective (mainly using FireTOS, probably): very little (you won't see much difference). From a developer's (my, personal and - of course biased) perspective: a lot.

- BaS_gcc provides "true" ST RAM (as far as possible using the ColdFire's MMU):
  The FireBee has two (dynamic) RAM areas: 512 MB of "ColdFire" RAM directly connected to the ColdFire chip and another 128M of DDR RAM attached to the FPGA. With original BaS, the ColdfireRAM is the only RAM you have direct access to and it resides linearily from address 0 to mem end. Original BaS only maps in (one single) window into the FPGA RAM where TOS expects to find its video RAM.

  BaS_gcc uses (again, using the ColdFire MMU) full 14 MB of the FPGA RAM as ST RAM and maps the Coldfire RAM as alternate (fast) RAM.
  
  This has benefits as well as drawbacks. First, the drawback: it's slower. The video circuit (an extended Falcon Videl, basically) that resides in the FPGA requires to constantly read the video (FPGA) memory to feed video data to the monitor. In the FPGA logic, this feed has priority and only leaves limited RAM bandwidth to the CPU. The more screen memory the FPGA needs to feed (higher video resolutions and bit depth), the slower the FPGA RAM will become.
  The second reason why it is slower is that access to ST RAM needs to go through the (32 MHz) FlexBus. But as Atari users, we are used to that (it's very similar on a TT or on a Falcon, where RAM bandwidth is also dependend on video resolution).
  
  The benefit: it is more compatible. If you have a TOS program that switches two screen buffers back and forth, for example, BaS has to map in and out FPGA memory pages using the MMU and needs to copy screen contents from Coldfire memory to FPGA memory and back behind the scenes (because your innocent TOS program has prepared the second screen in what it thought was ST RAM which in fact wasn't). While this works (somehow) it will result in strange delays during video address switching. BaS_gcc just does the same as an original Falcon and since all ST RAM is FPGA RAM, there is nothing to copy around.

- BaS_gcc only requires freely available OpenSource tools and no legacy toolchain (FreeScale CodeWarrior) to build as BaS does.
  These tools are available for all major platforms (Windows, Linux, Mac OS), and BaS_gcc has been tested to compile fine on all of these, so you are not limited to any specific configurations

- BaS_gcc enables (fast) networking using EmuTOS + FreeMiNT because it provides the ColdFire MDMA (multi channel DMA) and a suitable FEC driver. This way, you have a fully native OpenSource ColdFire environment without any slowdown through m68k emulation. Of course "legacy" (TOS) programs have to be compiled for this environment or can only used with m68k emulation (https://github.com/vinriviere/68kemu , for example), but if you intend to use the FireBee for new development, this is the fastest setup you can get with full access for all sources compilable with open source toolchains. A true OpenSorce environment.

[![Build Status](https://travis-ci.org/firebee-org/BaS_gcc.svg?branch=master)](https://travis-ci.org/firebee-org/BaS_gcc)
