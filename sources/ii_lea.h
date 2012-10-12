//-------------------------------------------------------------------
// lea
//-------------------------------------------------------------------
.text
ii_lea_lset:.macro
	ii_lset_dxu 4,f0			// lea d8(a0,dy.w),a0-a7
	ii_lset_dxu 4,f1			// lea d8(a1,dy.w),a0-a7
	ii_lset_dxu 4,f2			// lea d8(a2,dy.w),a0-a7
	ii_lset_dxu 4,f3			// lea d8(a3,dy.w),a0-a7
	ii_lset_dxu 4,f4			// lea d8(a4,dy.w),a0-a7
	ii_lset_dxu 4,f5			// lea d8(a5,dy.w),a0-a7
	ii_lset_dxu 4,f6			// lea d8(a6,dy.w),a0-a7
	ii_lset_dxu 4,f7			// lea d8(a7,dy.w),a0-a7
	ii_lset_dxu 4,fb			// lea d8(pc,dy.w),a0-a7
.endm

//---------------------------------------------------------------------------------------------
// function
//---------------------------------------------------------------------------------------------
ii_lea_sub:.macro
ii_0x4\1\2:
#ifdef	halten_lea
	halt
#endif
	move.l	\4,a1
	jsr		ewf
	move.l	a1,\3
	ii_end
.endm
ii_lea_func:.macro
//lea d8(ax,dy.w),a0-a7
	ii_lea_sub	1,f0,a0_off(a7),a0_off(a7)
	ii_lea_sub	1,f1,a0_off(a7),a1_off(a7)
	ii_lea_sub	1,f2,a0_off(a7),a2
	ii_lea_sub	1,f3,a0_off(a7),a3
	ii_lea_sub	1,f4,a0_off(a7),a4
	ii_lea_sub	1,f5,a0_off(a7),a5
	ii_lea_sub	1,f6,a0_off(a7),a6
	ii_lea_sub	1,f7,a0_off(a7),usp
	ii_lea_sub	3,f0,a1_off(a7),a0_off(a7)
	ii_lea_sub	3,f1,a1_off(a7),a1_off(a7)
	ii_lea_sub	3,f2,a1_off(a7),a2
	ii_lea_sub	3,f3,a1_off(a7),a3
	ii_lea_sub	3,f4,a1_off(a7),a4
	ii_lea_sub	3,f5,a1_off(a7),a5
	ii_lea_sub	3,f6,a1_off(a7),a6
	ii_lea_sub	3,f7,a1_off(a7),usp
	ii_lea_sub	5,f0,a2,a0_off(a7)
	ii_lea_sub	5,f1,a2,a1_off(a7)
	ii_lea_sub	5,f2,a2,a2
	ii_lea_sub	5,f3,a2,a3
	ii_lea_sub	5,f4,a2,a4
	ii_lea_sub	5,f5,a2,a5
	ii_lea_sub	5,f6,a2,a6
	ii_lea_sub	5,f7,a2,usp
	ii_lea_sub	7,f0,a3,a0_off(a7)
	ii_lea_sub	7,f1,a3,a1_off(a7)
	ii_lea_sub	7,f2,a3,a2
	ii_lea_sub	7,f3,a3,a3
	ii_lea_sub	7,f4,a3,a4
	ii_lea_sub	7,f5,a3,a5
	ii_lea_sub	7,f6,a3,a6
	ii_lea_sub	7,f7,a3,usp
	ii_lea_sub	9,f0,a4,a0_off(a7)
	ii_lea_sub	9,f1,a4,a1_off(a7)
	ii_lea_sub	9,f2,a4,a2
	ii_lea_sub	9,f3,a4,a3
	ii_lea_sub	9,f4,a4,a4
	ii_lea_sub	9,f5,a4,a5
	ii_lea_sub	9,f6,a4,a6
	ii_lea_sub	9,f7,a4,usp
	ii_lea_sub	b,f0,a5,a0_off(a7)
	ii_lea_sub	b,f1,a5,a1_off(a7)
	ii_lea_sub	b,f2,a5,a2
	ii_lea_sub	b,f3,a5,a3
	ii_lea_sub	b,f4,a5,a4
	ii_lea_sub	b,f5,a5,a5
	ii_lea_sub	b,f6,a5,a6
	ii_lea_sub	b,f7,a6,usp
	ii_lea_sub	d,f0,a6,a0_off(a7)
	ii_lea_sub	d,f1,a6,a1_off(a7)
	ii_lea_sub	d,f2,a6,a2
	ii_lea_sub	d,f3,a6,a3
	ii_lea_sub	d,f4,a6,a4
	ii_lea_sub	d,f5,a6,a5
	ii_lea_sub	d,f6,a6,a6
	ii_lea_sub	d,f7,a6,usp
	ii_lea_sub	f,f0,usp,a0_off(a7)
	ii_lea_sub	f,f1,usp,a1_off(a7)
	ii_lea_sub	f,f2,usp,a2
	ii_lea_sub	f,f3,usp,a3
	ii_lea_sub	f,f4,usp,a4
	ii_lea_sub	f,f5,usp,a5
	ii_lea_sub	f,f6,usp,a6
	ii_lea_sub	f,f7,usp,usp
// lea d8(pc,dy.w),az
	ii_lea_sub	1,fb,a0_off(a7),a0
	ii_lea_sub	3,fb,a1_off(a7),a0
	ii_lea_sub	5,fb,a2,a0
	ii_lea_sub	7,fb,a3,a0
	ii_lea_sub	9,fb,a4,a0
	ii_lea_sub	b,fb,a5,a0
	ii_lea_sub	d,fb,a6,a0
	ii_lea_sub	f,fb,usp,a0
.endm