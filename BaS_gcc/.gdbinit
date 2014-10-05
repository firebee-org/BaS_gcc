#set disassemble-next-line on
define tr
	#!killall m68k-bdm-gdbserver
	target remote | m68k-bdm-gdbserver pipe /dev/bdmcf3
	#target remote localhost:1234
	#target remote | m68k-bdm-gdbserver pipe /dev/tblcf1
	#target dbug /dev/ttyS0
	#monitor bdm-reset
end
define tbtr
	target remote | m68k-bdm-gdbserver pipe /dev/tblcf3
end
source mcf5474.gdb
set breakpoint auto-hw


