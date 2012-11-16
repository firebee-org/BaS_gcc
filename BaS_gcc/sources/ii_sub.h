//--------------------------------------------------------------------
// sub
//--------------------------------------------------------------------
/*****************************************************************************************/
//--------------------------------------------------------------------
// byt
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sub.b	#im,dx
//--------------------------------------------------------------------
subbir_macro:.macro
#ifdef	halten_sub
	halt
#endif
	move.w		(a0)+,d0
	extb.l		d0
	mvs.b		\2,d1
	sub.l		d0,d1
	set_cc0
	move.b		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	ea,dx
//--------------------------------------------------------------------
subdd:.macro
#ifdef	halten_sub
	halt
#endif
	mvs.\3		\1,d0
	mvs.\3		\2,d1
	sub.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	ea(l)->dy(w),dx z.B. für USP
//--------------------------------------------------------------------
subddd:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		\1,a1
	mvs.\3		a1,d0
	mvs.\3		\2,d1
	sub.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	(ea)->dy,dx
//--------------------------------------------------------------------
subdda:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		\1,a1
	mvs.\3		(a1),d0
	mvs.\3		\2,d1
	sub.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	ea->ay,(ay)+,dx
//--------------------------------------------------------------------
subddai:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		\1,a1
	mvs.\3		(a1)+,d0
	move.l		a1,\1
	mvs.\3		\2,d1
	sub.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	ea->ay,-(ay),dx
//--------------------------------------------------------------------
subddad:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		\1,a1
	mvs.\3		-(a1),d0
	move.l		a1,\1
	mvs.\3		\2,d1
	sub.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	d16(ay),dx
//--------------------------------------------------------------------
subd16ad:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		\1,a1
	mvs.w		(a0)+,d0
	add.l		d0,a1
	mvs.\3		(a1),d0		
	mvs.\3		\2,d1
	sub.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	d8(ay,dy),dx
//--------------------------------------------------------------------
subd8ad:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		\1,a1
	jsr			ewf
.ifc	\3,l
	move.l		(a1),d0
	move.l		\2,d1
.else
	mvs.\3		(a1),d0
	mvs.\3		\2,d1
.endif
	sub.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	xxx.w,dx
//--------------------------------------------------------------------
subxwd:.macro
#ifdef	halten_sub
	halt
#endif
	move.w		(a0)+,a1
	mvs.\3		(a1),d0
	mvs.\3		\2,d1
	sub.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	xxx.l,dx
//--------------------------------------------------------------------
subxld:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		(a0)+,a1
	mvs.\3		(a1),d0
	mvs.\3		\2,d1
	sub.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	d16(pc),dx
//--------------------------------------------------------------------
subd16pcd:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		a0,a1
	mvs.w		(a0)+,d0
	add.l		d0,a1
	mvs.\3		(a1),d0
	mvs.\3		\2,d1
	sub.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	d8(pc,dy),dx
//--------------------------------------------------------------------
subd8pcd:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		a0,a1
	jsr			ewf
.ifc	\3,l
	move.l		(a1),d0
	move.l		\2,d1
.else
	mvs.\3		(a1),d0
	mvs.\3		\2,d1
.endif
	sub.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
//  sub dy,ea
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// // sub	(ea)->dy,dx
//--------------------------------------------------------------------
subeda:.macro
#ifdef	halten_sub
	halt
#endif
	mvs.\3		\1,d0
	move.l		\2,a1
	mvs.\3		(a1),d1
	sub.l		d0,d1
	set_cc0
	move.\3		d1,(a1)
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	dx,ea->ay,(ay)+
//--------------------------------------------------------------------
subedai:.macro
#ifdef	halten_sub
	halt
#endif
	mvs.\3		\1,d0
	move.l		\2,a1
	mvs.\3		(a1),d1
	sub.l		d0,d1
	set_cc0
	move.\3		d1,(a1)+
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	dx,ea->ay,(ay)+
//--------------------------------------------------------------------
subedaid:.macro
#ifdef	halten_sub
	halt
#endif
	mvs.\3		\1,d0
	mvs.\3		\2,d1
	sub.l		d0,d1
	set_cc0
	move.\3		d1,\2+
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	dx,ea->ay,-(ay)
//--------------------------------------------------------------------
subedad:.macro
#ifdef	halten_sub
	halt
#endif
	mvs.\3		\1,d0
	move.l		\2,a1
	mvs.\3		-(a1),d1
	move.l		a1,\2
	sub.l		d0,d1
	set_cc0
	move.\3		d1,(a1)
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	dx,ea->ay,-(ay)
//--------------------------------------------------------------------
subedadd:.macro
#ifdef	halten_sub
	halt
#endif
	mvs.\3		\1,d0
	mvs.\3		-\2,d1
	sub.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	dx,d16(ay)
//--------------------------------------------------------------------
sube16ad:.macro
#ifdef	halten_sub
	halt
#endif
	mvs.\3		\1,d0
	move.l		\2,a1
	mvs.w		(a0)+,d1
	add.l		d1,a1
	mvs.\3		(a1),d1		
	sub.l		d0,d1
	set_cc0
	move.\3		d1,(a1)
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	dx,d8(ay,dy)
//--------------------------------------------------------------------
sube8ad:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		\2,a1
	jsr			ewf
.ifc	\3,l
	move.l		(a1),d1
	move.l		\1,d0
.else
	mvs.\3		(a1),d1
	mvs.\3		\1,d0
.endif
	sub.l		d0,d1
	set_cc0
	move.\3		d1,(a1)
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	dx,xxx.w
//--------------------------------------------------------------------
subxwe:.macro
#ifdef	halten_sub
	halt
#endif
	mvs.\3		\1,d0
	move.w		(a0)+,a1
	mvs.\3		(a1),d1
	sub.l		d0,d1
	set_cc0
	move.\3		d1,(a1)
	ii_end
	.endm;
//--------------------------------------------------------------------
// // sub	dx,xxx.l
//--------------------------------------------------------------------
subxle:.macro
#ifdef	halten_sub
	halt
#endif
	mvs.\3		\1,d0
	move.l		(a0)+,a1
	mvs.\3		(a1),d1
	sub.l		d0,d1
	set_cc0
	move.\3		d1,(a1)
	ii_end
	.endm;
/******************************************************/
// adress register
/******************************************************/
//--------------------------------------------------------------------
// // suba.w	ea,ax
//--------------------------------------------------------------------
subaw:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		a0,pc_off(a7)			// pc auf next
	movem.l		(a7),d0/d1/a0/a1		// register zurpück
	mvs.w		\1,d0
	suba.l		d0,\2
	move.l		d0_off(a7),d0
	lea			ii_ss(a7),a7			// stack erhöhen
	rte
	.endm;
//--------------------------------------------------------------------
// sub.w ea,usp
//--------------------------------------------------------------------
subawa7:.macro
#ifdef	halten_sub
	halt
#endif
	mvs.w		\1,d0
	move.l		usp,a1
	sub.l		d0,a1
	move.l		a1,usp
	ii_end
	.endm;
//--------------------------------------------------------------------
// // suba.w	usp?,ax
//--------------------------------------------------------------------
subawu:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		a0,pc_off(a7)			// pc auf next
	movem.l		(a7),d0/d1/a0/a1		// register zurpück
	move.l		a7,_a7_save
	move.l		usp,a7
	move.l		\1,d0
	suba.l		d0,\2
	move.l		a7,usp
	move.l		_a7_save,a7
	move.l		d0_off(a7),d0
	lea			ii_ss(a7),a7			// stack erhöhen
	rte
	.endm;
//--------------------------------------------------------------------
// // suba.w	usp?,usp
//--------------------------------------------------------------------
subawua7:.macro
	subawu		\1,\2
	.endm;
//--------------------------------------------------------------------
// // suba.w	d16(ay),ax
//--------------------------------------------------------------------
subawd16a:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		\1,a1
	mvs.w		(a0)+,d0
	adda.l		d0,a1
	mvs.w		(a1),d0
	move.l		\2,a1
	sub.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // suba.w	d8(ay,dy),ax
//--------------------------------------------------------------------
subawd8a:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		\1,a1
	jsr			ewf
	mvs.w		(a1),d0
	move.l		\2,a1
	sub.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // suba.w	xxx.w,ax
//--------------------------------------------------------------------
subawxwax:.macro
#ifdef	halten_sub
	halt
#endif
	move.w		(a0)+,a1
	mvs.w		(a1),d0
	move.l		\2,a1
	suba.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // suba.w	xxx.l,ax
//--------------------------------------------------------------------
subawxlax:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		(a0)+,a1
	mvs.w		(a1),d0
	move.l		\2,a1
	suba.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // suba.w	d16(pc),ax
//--------------------------------------------------------------------
subawd16pcax:.macro
#ifdef	halten_sub
	halt
#endif
	move.w		(a0)+,a1
	adda.l		a0,a1
	mvs.w		(a1),d0
	move.l		\2,a1
	suba.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // suba.w	d8(pc,dy),ax
//--------------------------------------------------------------------
subawd8pcax:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		a0,a1
	jsr			ewf
	mvs.w		(a1),d0
	move.l		\2,a1
	sub.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // suba.w	#im,ax
//--------------------------------------------------------------------
subawim:.macro
#ifdef	halten_sub
	halt
#endif
	mvs.w		\1,d0
	move.l		\2,a1
	sub.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // suba.l	d8(ay,dy),ax
//--------------------------------------------------------------------
subald8a:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		\1,a1
	jsr			ewf
	move.l		(a1),d0
	move.l		\2,a1
	sub.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // suba.l	d8(pc,dy),ax
//--------------------------------------------------------------------
subakd8pcax:.macro
#ifdef	halten_sub
	halt
#endif
	move.l		a0,a1
	jsr			ewf
	move.l		(a1),d0
	move.l		\2,a1
	sub.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//*****************************************************************************************
// subx 
//*****************************************************************************************
//--------------------------------------------------------------------
// // subx	dy,dx
//--------------------------------------------------------------------
subdx:.macro
#ifdef	halten_sub
	halt
#endif
	move.b		sr_off+1(a7),d0			//ccr holen
	move		d0,ccr					//setzen		
	mvs.\3		\2,d0
	mvs.\3		\1,d1
	subx.l		d0,d1
	set_cc0
	move.\3		d1,\1
	ii_end
	.endm;
//--------------------------------------------------------------------
// // subx	-(ay),-(ax)
//--------------------------------------------------------------------
subdax:.macro
#ifdef	halten_sub
	halt
#endif
	move.b		sr_off+1(a7),d0			//ccr holen
	move		d0,ccr					//setzen		
	move.l		\1,a1
.ifc	\3,l
	move.l		-(a1),d0
.else
	mvs.\3		-(a1),d0
.endif
	move.l		\2,a1
.ifc	\3,l
	move.l		-(a1),d0
.else
	mvs.\3		-(a1),d1
.endif
	subx.l		d0,d1
	set_cc0
	move.\3		d1,(a1)
	ii_end
	.endm;
//--------------------------------------------------------------------
