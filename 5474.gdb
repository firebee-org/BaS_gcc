#
# GDB Init script for the Coldfire 5206 processor.
#
# The main purpose of this script is to configure the 
# DRAM controller so code can be loaded.
#
#

define addresses
set $mbar  = 0xFF000000
set $vbr   = 0x00000000
set $rambar0 = 0xFF100000
set $rambar1 = 0xFF101000
end

#
# Setup the DRAM controller.
#

define setup-dram
# Init CS0 (BootFLASH @ E000_0000 - E07F_FFFF 8Mbytes)
set *((long *) 0xFF000500) = 0xE0000000
set *((long *) 0xFF000508) = 0x00001180
set *((long *) 0xFF000504) = 0x007F0001

# SDRAM Initialization @ 0000_0000 - 1FFF_FFFF 512Mbytes
# SDRAMDS configuration
set *((long *) 0xFF000004) = 0x000002AA
# SDRAM CS0 configuration (128Mbytes 0000_0000 - 07FF_FFFF)	
set *((long *) 0xFF000020) = 0x0000001A
# SDRAM CS1 configuration (128Mbytes 0800_0000 - 0FFF_FFFF)
set *((long *) 0xFF000024) = 0x0800001A
# SDRAM CS2 configuration (128Mbytes 1000_0000 - 17FF_FFFF)
set *((long *) 0xFF000028) = 0x1000001A
# SDRAM CS3 configuration (128Mbytes 1800_0000 - 1FFF_FFFF)
set *((long *) 0xFF00002C) = 0x1800001A
# SDCFG1
set *((long *) 0xFF000108) = 0x53722938
# SDCFG2
set *((long *) 0xFF00010C) = 0x24330000
end

#
# Wake up the board
#

define initBoard
addresses
setup-dram
end