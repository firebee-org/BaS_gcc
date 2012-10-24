#
# GDB Init script for the Coldfire 5474 processor (Firebee board).
#
# The main purpose of this script is to configure the 
# DRAM controller so code can be loaded.
#
#

define addresses
set $vbr   = 0x00000000
set $mbar  = 0xFF000000
set $rambar0 = 0xFF100000
set $rambar1 = 0xFF101000
end

#
# Setup the DRAM controller.
#

define setup-dram
# Init CS0 (BootFLASH @ E000_0000 - E07F_FFFF 8Mbytes)
set *((long *) 0xFF000500) = 0xE0000000		# flash address
set *((long *) 0xFF000508) = 0x00041180		# 16 bit 4ws aa
set *((long *) 0xFF000504) = 0x007F0001		# 8MB on

# set *((long *) 0xFF00050C) = 0xFFF00000		# ATARI I/O address

# SDRAM Initialization @ 0000_0000 - 1FFF_FFFF 512Mbytes
set *((long *) 0xFF000004) = 0x000002AA		# SDRAMDS configuration
set *((long *) 0xFF000020) = 0x0000001A		# SDRAM CS0 configuration (128Mbytes 0000_0000 - 07FF_FFFF)	
set *((long *) 0xFF000024) = 0x0800001A		# SDRAM CS1 configuration (128Mbytes 0800_0000 - 0FFF_FFFF)
set *((long *) 0xFF000028) = 0x1000001A		# SDRAM CS2 configuration (128Mbytes 1000_0000 - 17FF_FFFF)
set *((long *) 0xFF00002C) = 0x1800001A		# SDRAM CS3 configuration (128Mbytes 1800_0000 - 1FFF_FFFF)
set *((long *) 0xFF000108) = 0x73622830		# SDCFG1
set *((long *) 0xFF00010C) = 0x46770000		# SDCFG2


set *((long *) 0xFF000104) = 0xE10D0002		# SDCR + IPALL
set *((long *) 0xFF000100) = 0x40010000		# SDMR (write to LEMR)
set *((long *) 0xFF000100) = 0x048D0000		# SDMR (write to LMR)
set *((long *) 0xFF000104) = 0xE10D0002		# SDCR + IPALL
set *((long *) 0xFF000104) = 0xE10D0004		# SDCR + IREF (first refresh)
set *((long *) 0xFF000104) = 0xE10D0004		# SDCR + IREF (first refresh)
set *((long *) 0xFF000100) = 0x008D0000		# SDMR (write to LMR)
set *((long *) 0xFF000104) = 0x710D0F00		# SDCR (lock SDMR and enable refresh)
end

#
# Wake up the board
#

define initBoard
addresses
setup-dram
end

initBoard