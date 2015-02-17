/*
 * needed for ELF compilation of x86emu which uses 64 bit multiplication and division. This
 * is implemented in libgcc.a. However, the ELF compiler emits leading underscores and libgcc
 * is not compiled like that - we must reimplement the symbols.
 *
 * FIXME: This is a quirk and should be replaced by BaS' own implementation later
 */
		.global		___divdi3
		.extern		__divdi3
___divdi3:	jmp		__divdi3

		.global		___muldi3
		.extern		__muldi3
___muldi3:	jmp		__muldi3

		.global		___moddi3
		.extern		__moddi3
___moddi3:	jmp		__moddi3

		.global		___udivdi3
		.extern		__udivdi3
___udivdi3:	jmp		__udivdi3

		.global		___umoddi3
		.extern		__umoddi3
___umoddi3:	jmp		__umoddi3
