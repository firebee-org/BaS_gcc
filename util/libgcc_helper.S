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

		.global		___divdf3
		.extern		__divdf3
___divdf3:	jmp		__divdf3

		.global		___muldf3
		.extern		__muldf3
___muldf3:	jmp		__muldf3

		.global		___subsf3
		.extern		__subsf3
___subsf3:	jmp		__subsf3

		.global		___floatsidf
		.extern		__floatsidf
___floatsidf:	jmp		__floatsidf

		.global		___floatsisf
		.extern		__floatsisf
___floatsisf:	jmp		__floatsisf

		.global		___extendsfdf2
		.extern		__extendsfdf2
___extendsfdf2:	jmp		__extendsfdf2

		.global		___floatunsidf
		.extern		__floatunsidf
___floatunsidf:	jmp		__floatunsidf

		.global		___truncdfsf2
		.extern		__truncdfsf2
___truncdfsf2:	jmp		__truncdfsf2

		.global		___ltsf2
		.extern		__ltsf2
___ltsf2:	jmp		__ltsf2

		.global		___fixdfsi
		.extern		__fixdfsi
___fixdfsi:	jmp		__fixdfsi

		.global		___fixsfsi
		.extern		__fixsfsi
___fixsfsi:	jmp		__fixsfsi
