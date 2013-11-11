#
# GDB Init script for the Coldfire 5474 processor (firebee).
#

define addresses
set $vbr   = 0x00000000
#monitor bdm-ctl-set 0x0801 0x00000000
 
set $mbar  = 0xFF000000
#monitor bdm-ctl-set 0x0C0F 0xFF000000

set $rambar0 = 0xFF100000
#monitor bdm-ctl-set 0x0C04 0xFF100007

set $rambar1 = 0xFF101000
#monitor bdm-ctl-set 0x0C05 0xFF101001
end

#
# Setup the DRAM controller.
#

define setup-dram
# Init CS0 (BootFLASH @ E000_0000 - E07F_FFFF 8Mbytes)
set *((long *) 0xFF000500) = 0xE0000000		
set *((long *) 0xFF000508) = 0x00041180		
set *((long *) 0xFF000504) = 0x007F0001		

# set *((long *) 0xFF00050C) = 0xFFF00000		# ATARI I/O address

# SDRAM Initialization @ 0000_0000 - 1FFF_FFFF 512Mbytes
set *((long *) 0xFF000004) = 0x000002AA		
set *((long *) 0xFF000020) = 0x0000001A		
set *((long *) 0xFF000024) = 0x0800001A		
set *((long *) 0xFF000028) = 0x1000001A		
set *((long *) 0xFF00002C) = 0x1800001A		
set *((long *) 0xFF000108) = 0x73622830		
set *((long *) 0xFF00010C) = 0x46770000		


set *((long *) 0xFF000104) = 0xE10D0002		
set *((long *) 0xFF000100) = 0x40010000		
set *((long *) 0xFF000100) = 0x048D0000		
set *((long *) 0xFF000104) = 0xE10D0002		
set *((long *) 0xFF000104) = 0xE10D0004		
set *((long *) 0xFF000104) = 0xE10D0004		
set *((long *) 0xFF000100) = 0x008D0000		
set *((long *) 0xFF000104) = 0x710D0F00		
end

define cu
!killall m68k-bdm-gdbserver
end

#
# Wake up the board
#

define ib
	addresses
	setup-dram
end

ib
tr
load
