//--------------------------------------------------------------------
// add
//--------------------------------------------------------------------
/*****************************************************************************************/
//--------------------------------------------------------------------
// byt
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// add.b	#im,dx
//--------------------------------------------------------------------
addbir_macro:.macro
	move.w		(a0)+,d0
	extb.l		d0
	mvs.b		\2,d1
	add.l		d0,d1
	set_cc0
	move.b		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	ea,dx
//--------------------------------------------------------------------
adddd:.macro
#ifdef	halten_add
	halt
#endif
	mvs.\3		\1,d0
	mvs.\3		\2,d1
	add.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	ea,dx (first ea->a1 z.B. für a0,a1,USP)
//--------------------------------------------------------------------
addddd:.macro
#ifdef	halten_add
	halt
#endif
	move.l		\1,a1
	mvs.\3		a1,d0
	mvs.\3		\2,d1
	add.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	(ea),dx (first ea->a1 z.B. für a0,a1,USP)
//--------------------------------------------------------------------
adddda:.macro
#ifdef	halten_add
	halt
#endif
	move.l		\1,a1
	mvs.\3		(a1),d0
	mvs.\3		\2,d1
	add.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	(ay)+,dx (first ea->a1 z.B. für a0,a1,USP)
//--------------------------------------------------------------------
addddai:.macro
#ifdef	halten_add
	halt
#endif
	move.l		\1,a1
	mvs.\3		(a1)+,d0
	move.l		a1,\1
	mvs.\3		\2,d1
	add.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	-(ay),dx (first ea->a1 z.B. für a0,a1,USP)
//--------------------------------------------------------------------
addddad:.macro
#ifdef	halten_add
	halt
#endif
	move.l		\1,a1
	mvs.\3		-(a1),d0
	move.l		a1,\1
	mvs.\3		\2,d1
	add.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	d16(ay),dx
//--------------------------------------------------------------------
addd16ad:.macro
#ifdef	halten_add
	halt
#endif
	move.l		\1,a1
	mvs.w		(a0)+,d0
	add.l		d0,a1
	mvs.\3		(a1),d0		
	mvs.\3		\2,d1
	add.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	d8(ay,dy),dx
//--------------------------------------------------------------------
addd8ad:.macro
#ifdef	halten_add
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
	add.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	xxx.w,dx
//--------------------------------------------------------------------
addxwd:.macro
#ifdef	halten_add
	halt
#endif
	move.w		(a0)+,a1
	mvs.\3		(a1),d0
	mvs.\3		\2,d1
	add.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	xxx.l,dx
//--------------------------------------------------------------------
addxld:.macro
#ifdef	halten_add
	halt
#endif
	move.l		(a0)+,a1
	mvs.\3		(a1),d0
	mvs.\3		\2,d1
	add.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	d16(pc),dx
//--------------------------------------------------------------------
addd16pcd:.macro
#ifdef	halten_add
	halt
#endif
	move.l		a0,a1
	mvs.w		(a0)+,d0
	add.l		d0,a1
	mvs.\3		(a1),d0
	mvs.\3		\2,d1
	add.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	d8(pc,dy),dx
//--------------------------------------------------------------------
addd8pcd:.macro
#ifdef	halten_add
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
	add.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
//  add dy,ea
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// // add	dx,(ay)  (first ea->a1 z.B. für a0,a1,USP)
//--------------------------------------------------------------------
addeda:.macro
#ifdef	halten_add
	halt
#endif
	mvs.\3		\1,d0
	move.l		\2,a1
	mvs.\3		(a1),d1
	add.l		d0,d1
	set_cc0
	move.\3		d1,(a1)
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	dx,(ay)+  (first ea->a1 z.B. für a0,a1,USP)
//--------------------------------------------------------------------
addedai:.macro
#ifdef	halten_add
	halt
#endif
	mvs.\3		\1,d0
	move.l		\2,a1
	mvs.\3		(a1),d1
	add.l		d0,d1
	set_cc0
	move.\3		d1,(a1)+
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	dx,(ay)+  
//--------------------------------------------------------------------
addedaid:.macro
#ifdef	halten_add
	halt
#endif
	mvs.\3		\1,d0
	mvs.\3		\2,d1
	add.l		d0,d1
	set_cc0
	move.\3		d1,\2+
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	dx,-(ay)
//--------------------------------------------------------------------
addedad:.macro
#ifdef	halten_add
	halt
#endif
	mvs.\3		\1,d0
	move.l		\2,a1
	mvs.\3		-(a1),d1
	move.l		a1,\2
	add.l		d0,d1
	set_cc0
	move.\3		d1,(a1)
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	dx,-(ay)
//--------------------------------------------------------------------
addedadd:.macro
#ifdef	halten_add
	halt
#endif
	mvs.\3		\1,d0
	mvs.\3		-\2,d1
	add.l		d0,d1
	set_cc0
	move.\3		d1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	dx,d16(ay)
//--------------------------------------------------------------------
adde16ad:.macro
#ifdef	halten_add
	halt
#endif
	mvs.\3		\1,d0
	move.l		\2,a1
	mvs.w		(a0)+,d1
	add.l		d1,a1
	mvs.\3		(a1),d1		
	add.l		d0,d1
	set_cc0
	move.\3		d1,(a1)
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add.w	d8(ay,dy),dx
//--------------------------------------------------------------------
adde8ad:.macro
#ifdef	halten_add
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
	add.l		d0,d1
	set_cc0
	move.\3		d1,(a1)
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	dx,xxx.w
//--------------------------------------------------------------------
addxwe:.macro
#ifdef	halten_add
	halt
#endif
	mvs.\3		\1,d0
	move.w		(a0)+,a1
	mvs.\3		(a1),d1
	add.l		d0,d1
	set_cc0
	move.\3		d1,(a1)
	ii_end
	.endm;
//--------------------------------------------------------------------
// // add	dx,xxx.l
//--------------------------------------------------------------------
addxle:.macro
#ifdef	halten_add
	halt
#endif
	mvs.\3		\1,d0
	move.l		(a0)+,a1
	mvs.\3		(a1),d1
	add.l		d0,d1
	set_cc0
	move.\3		d1,(a1)
	ii_end
	.endm;
/******************************************************/
// adress register
/******************************************************/
//--------------------------------------------------------------------
// // adda.w	ea,ax (ea = dx;ax;(ax);(ax)+,-(ax)
//--------------------------------------------------------------------
addaw:.macro
#ifdef	halten_add
	halt
#endif
	move.l		a0,pc_off(a7)			// pc auf next
	movem.l		(a7),d0/d1/a0/a1		// register zurpück
	mvs.w		\1,d0
	adda.l		d0,\2
	move.l		d0_off(a7),d0
	lea			ii_ss(a7),a7			// stack erhöhen
	rte
	.endm;
//--------------------------------------------------------------------
// add.w ea,usp
//--------------------------------------------------------------------
addawa7:.macro
#ifdef	halten_add
	halt
#endif
	mvs.w		\1,d0
	move.l		usp,a1
	add.l		d0,a1
	move.l		a1,usp
	ii_end
	.endm;
//--------------------------------------------------------------------
// // adda.w	ea,usp (ea = dx;ax;(ax);(ax)+,-(ax)
//--------------------------------------------------------------------
addawu:.macro
#ifdef	halten_add
	halt
#endif
	move.l		a0,pc_off(a7)			// pc auf next
	movem.l		(a7),d0/d1/a0/a1		// register zurpück
	move.l		a7,_a7_save
	move.l		usp,a7
	move.l		\1,d0
	adda.l		d0,\2
	move.l		a7,usp
	move.l		_a7_save,a7
	move.l		d0_off(a7),d0
	lea			ii_ss(a7),a7			// stack erhöhen
	rte
	.endm;
//--------------------------------------------------------------------
// // adda.w	ea,usp (ea = a7 => dx;ax;(ax);(ax)+,-(ax)
//--------------------------------------------------------------------
addawua7:.macro
	addawu		\1,\2
	.endm;
//--------------------------------------------------------------------
// // adda.w	d16(ay),ax
//--------------------------------------------------------------------
addawd16a:.macro
#ifdef	halten_add
	halt
#endif
	move.l		\1,a1
	mvs.w		(a0)+,d0
	adda.l		d0,a1
	mvs.w		(a1),d0
	move.l		\2,a1
	add.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // adda.w	d8(ay,dy),ax
//--------------------------------------------------------------------
addawd8a:.macro
#ifdef	halten_add
	halt
#endif
	move.l		\1,a1
	jsr			ewf
	mvs.w		(a1),d0
	move.l		\2,a1
	add.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // adda.w	xxx.w,ax
//--------------------------------------------------------------------
addawxwax:.macro
#ifdef	halten_add
	halt
#endif
	move.w		\1,a1
	mvs.w		(a1),d0
	move.l		\2,a1
	adda.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // adda.w	xxx.l,ax
//--------------------------------------------------------------------
addawxlax:.macro
#ifdef	halten_add
	halt
#endif
	move.l		\1,a1
	mvs.w		(a1),d0
	move.l		\2,a1
	adda.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // adda.w	d16(pc),ax
//--------------------------------------------------------------------
addawd16pcax:.macro
#ifdef	halten_add
	halt
#endif
	move.w		\1,a1
	adda.l		a0,a1
	mvs.w		(a1),d0
	move.l		\2,a1
	adda.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // adda.w	d8(pc,dy),ax
//--------------------------------------------------------------------
addawd8pcax:.macro
#ifdef	halten_add
	halt
#endif
	move.l		a0,a1
	jsr			ewf
	mvs.w		(a1),d0
	move.l		\2,a1
	adda.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // adda.w	#im,ax
//--------------------------------------------------------------------
addawim:.macro
#ifdef	halten_add
	halt
#endif
	mvs.w		\1,d0
	move.l		\2,a1
	adda.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // adda.l	d8(ay,dy),ax
//--------------------------------------------------------------------
addald8a:.macro
#ifdef	halten_add
	halt
#endif
	move.l		\1,a1
	jsr			ewf
	move.l		(a1),d0
	move.l		\2,a1
	adda.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//--------------------------------------------------------------------
// // adda.l	d8(pc,dy),ax
//--------------------------------------------------------------------
addakd8pcax:.macro
#ifdef	halten_add
	halt
#endif
	move.l		a0,a1
	jsr			ewf
	move.l		(a1),d0
	move.l		\2,a1
	adda.l		d0,a1
	move.l		a1,\2
	ii_end
	.endm;
//*****************************************************************************************
// addx 
//*****************************************************************************************
//--------------------------------------------------------------------
// // addx	dy,dx
//--------------------------------------------------------------------
adddx:.macro
#ifdef	halten_add
	halt
#endif
	move.b		sr_off+1(a7),d0			//ccr holen
	move		d0,ccr					//setzen		
	mvs.\3		\2,d0
	mvs.\3		\1,d1
	addx.l		d0,d1
	set_cc0
	move.\3		d1,\1
	ii_end
	.endm;
//--------------------------------------------------------------------
// // addx	-(ay),-(ax)
//--------------------------------------------------------------------
adddax:.macro
#ifdef	halten_add
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
	addx.l		d0,d1
	set_cc0
	move.\3		d1,(a1)
	ii_end
	.endm;
//--------------------------------------------------------------------
