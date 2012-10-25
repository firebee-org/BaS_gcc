set disassemble-next-line on
define tr
	target remote | m68k-atari-mint-gdbserver pipe /dev/bdmcf3
end
tr
source mcf5474.gdb


