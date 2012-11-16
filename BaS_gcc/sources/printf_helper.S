// assembler trampoline to let printf (compiled -mpcrel) indirectly reference __MBAR

			.global printf_helper
printf_helper:
			.extern		__MBAR
.wait_txready:
			move.w		__MBAR+0x8604,d2	// PSCSCR0 status register
			btst		#10,d2				// space left in TX fifo?
			beq.s		.wait_txready		// no, loop
			lea			__MBAR+0x860C,a0	// PSCSTB0 transmitter buffer register
			move.b		d0,(a0)				// send byte
			rts
