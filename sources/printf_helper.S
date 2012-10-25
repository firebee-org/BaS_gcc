// assembler trampoline to let printf (compiled -mpcrel) indirectly reference __MBAR

			.global printf_helper
printf_helper:
			.extern		__MBAR
			lea			__MBAR+0x860C,a0
			move.b		d0,(a0)
			rts
