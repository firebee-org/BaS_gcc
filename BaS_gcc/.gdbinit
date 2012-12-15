#set disassemble-next-line on
define tr
	target remote | m68k-bdm-gdbserver pipe /dev/bdmcf3
end
define tbtr
	target remote | m68k-bdm-gdbserver pipe /dev/tblcf3
end

tr
define run
	continue
end
source mcf5474.gdb

