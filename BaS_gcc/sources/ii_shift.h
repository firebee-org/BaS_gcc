/*****************************************************************************************/
// opertionen
/*****************************************************************************************/
ii_shift_lset:.macro
/******************************************************/
// byt
/******************************************************/
//  asx.b #,dx
	ii_lset_dx	\1,00						// 0x1.22 -> z.B. 1=d2=4 ->0xd07c -> 0xde7c			
	ii_lset_dx	\1,01			
	ii_lset_dx	\1,02			
	ii_lset_dx	\1,03			
	ii_lset_dx	\1,04			
	ii_lset_dx	\1,05			
	ii_lset_dx	\1,06			
	ii_lset_dx	\1,07			
// lsx.b #,dx
	ii_lset_dxu	\1,08			
	ii_lset_dxu	\1,09			
	ii_lset_dxu	\1,0a			
	ii_lset_dxu	\1,0b			
	ii_lset_dxu	\1,0c			
	ii_lset_dxu	\1,0d			
	ii_lset_dxu	\1,0e			
	ii_lset_dxu	\1,0f			
// roxx.b #,dx
	ii_lset_dx	\1,10			
	ii_lset_dx	\1,11			
	ii_lset_dx	\1,12			
	ii_lset_dx	\1,13			
	ii_lset_dx	\1,14			
	ii_lset_dx	\1,15			
	ii_lset_dx	\1,16			
	ii_lset_dx	\1,17			
// rox.b #,dx
	ii_lset_dx	\1,18			
	ii_lset_dx	\1,19			
	ii_lset_dx	\1,1a			
	ii_lset_dx	\1,1b			
	ii_lset_dx	\1,1c			
	ii_lset_dx	\1,1d			
	ii_lset_dx	\1,1e			
	ii_lset_dx	\1,1f			
// asx.b dy,dx
	ii_lset_dx	\1,20			
	ii_lset_dx	\1,21			
	ii_lset_dx	\1,22			
	ii_lset_dx	\1,23			
	ii_lset_dx	\1,24			
	ii_lset_dx	\1,25			
	ii_lset_dx	\1,26			
	ii_lset_dx	\1,27			
// lsx.b dy,dx
	ii_lset_dx	\1,28			
	ii_lset_dx	\1,29			
	ii_lset_dx	\1,2a			
	ii_lset_dx	\1,2b			
	ii_lset_dx	\1,2c			
	ii_lset_dx	\1,2d			
	ii_lset_dx	\1,2e			
	ii_lset_dx	\1,2f			
// roxx.dy,dx
	ii_lset_dx	\1,30			
	ii_lset_dx	\1,31			
	ii_lset_dx	\1,32			
	ii_lset_dx	\1,33			
	ii_lset_dx	\1,34			
	ii_lset_dx	\1,35			
	ii_lset_dx	\1,36			
	ii_lset_dx	\1,37			
// rox.b dy,dx
	ii_lset_dx	\1,38			
	ii_lset_dx	\1,39			
	ii_lset_dx	\1,3a			
	ii_lset_dx	\1,3b			
	ii_lset_dx	\1,3c			
	ii_lset_dx	\1,3d			
	ii_lset_dx	\1,3e			
	ii_lset_dx	\1,3f			
/******************************************************/
// word
/******************************************************/
// asx.w #x,dx
	ii_lset_dx	\1,40						// 0x1.22 -> z.B. 1=d2=4 ->0xd07c -> 0xde7c			
	ii_lset_dx	\1,41			
	ii_lset_dx	\1,42			
	ii_lset_dx	\1,43			
	ii_lset_dx	\1,44			
	ii_lset_dx	\1,45			
	ii_lset_dx	\1,46			
	ii_lset_dx	\1,47			
// lsx.w #,dx
	ii_lset_dx	\1,48			
	ii_lset_dx	\1,49			
	ii_lset_dx	\1,4a			
	ii_lset_dx	\1,4b			
	ii_lset_dx	\1,4c			
	ii_lset_dx	\1,4d			
	ii_lset_dx	\1,4e			
	ii_lset_dx	\1,4f			
// roxx.w #,dx
	ii_lset_dx	\1,50			
	ii_lset_dx	\1,51			
	ii_lset_dx	\1,52			
	ii_lset_dx	\1,53			
	ii_lset_dx	\1,54			
	ii_lset_dx	\1,55			
	ii_lset_dx	\1,56			
	ii_lset_dx	\1,57			
// rox.w #xdx
	ii_lset_dx	\1,58			
	ii_lset_dx	\1,59			
	ii_lset_dx	\1,5a			
	ii_lset_dx	\1,5b			
	ii_lset_dx	\1,5c			
	ii_lset_dx	\1,5d			
	ii_lset_dx	\1,5e			
	ii_lset_dx	\1,5f			
// asx.w dy,dx
	ii_lset_dx	\1,60			
	ii_lset_dx	\1,61			
	ii_lset_dx	\1,62			
	ii_lset_dx	\1,63			
	ii_lset_dx	\1,64			
	ii_lset_dx	\1,65			
	ii_lset_dx	\1,66			
	ii_lset_dx	\1,67			
// lsx.w dy,dx
	ii_lset_dx	\1,68			
	ii_lset_dx	\1,69			
	ii_lset_dx	\1,6a			
	ii_lset_dx	\1,6b			
	ii_lset_dx	\1,6c			
	ii_lset_dx	\1,6d			
	ii_lset_dx	\1,6e			
	ii_lset_dx	\1,6f			
// roxx.w dy,dx
	ii_lset_dx	\1,70			
	ii_lset_dx	\1,71			
	ii_lset_dx	\1,72			
	ii_lset_dx	\1,73			
	ii_lset_dx	\1,74			
	ii_lset_dx	\1,75			
	ii_lset_dx	\1,76			
	ii_lset_dx	\1,77			
// rox.w dy,dx
	ii_lset_dx	\1,78			
	ii_lset_dx	\1,79			
	ii_lset_dx	\1,7a			
	ii_lset_dx	\1,7b			
	ii_lset_dx	\1,7c			
	ii_lset_dx	\1,7d			
	ii_lset_dx	\1,7e			
	ii_lset_dx	\1,7f			
/******************************************************/
// long
/******************************************************/
// roxx.l #,dx
	ii_lset_dx	\1,90			
	ii_lset_dx	\1,91			
	ii_lset_dx	\1,92			
	ii_lset_dx	\1,93			
	ii_lset_dx	\1,94			
	ii_lset_dx	\1,95			
	ii_lset_dx	\1,96			
	ii_lset_dx	\1,97			
// rox.l #xdx
	ii_lset_dx	\1,98			
	ii_lset_dx	\1,99			
	ii_lset_dx	\1,9a			
	ii_lset_dx	\1,9b			
	ii_lset_dx	\1,9c			
	ii_lset_dx	\1,9d			
	ii_lset_dx	\1,9e			
	ii_lset_dx	\1,9f			
// roxx.l dy,dx
	ii_lset_dx	\1,b0			
	ii_lset_dx	\1,b1			
	ii_lset_dx	\1,b2			
	ii_lset_dx	\1,b3			
	ii_lset_dx	\1,b4			
	ii_lset_dx	\1,b5			
	ii_lset_dx	\1,b6			
	ii_lset_dx	\1,b7			
// rox.l dy,dx
	ii_lset_dx	\1,b8			
	ii_lset_dx	\1,b9			
	ii_lset_dx	\1,ba			
	ii_lset_dx	\1,bb			
	ii_lset_dx	\1,bc			
	ii_lset_dx	\1,bd			
	ii_lset_dx	\1,be			
	ii_lset_dx	\1,bf
//--------------------------------------------------------------------
// asr.w ea
	ii_lset_opea	\10,d			// (ax), (ax)+
	ii_lset_opea	\10,e			// -(ax),d16(ax)
	ii_lset_opeag	\10,f			// d8(ax,dy)
	lea	table+0x\10\2f8*4,a0
	move.l	#ii_0x\10f8,(a0)+		// xxx.w
	move.l	#ii_0x\10f9,(a0)+		// xxx.l
// asl.w ea
	ii_lset_opea	\11,d			// (ax), (ax)+
	ii_lset_opea	\11,e			// -(ax),d16(ax)
	ii_lset_opeag	\11,f			// d8(ax,dy)
	lea	table+0x\11\2f8*4,a0
	move.l	#ii_0x\11f8,(a0)+		// xxx.w
	move.l	#ii_0x\11f9,(a0)+		// xxx.l
// lsr.w ea
	ii_lset_opea	\12,d			// (ax), (ax)+
	ii_lset_opea	\12,e			// -(ax),d16(ax)
	ii_lset_opeag	\12,f			// d8(ax,dy)
	lea	table+0x\12\2f8*4,a0
	move.l	#ii_0x\12f8,(a0)+		// xxx.w
	move.l	#ii_0x\12f9,(a0)+		// xxx.l
// lsr.w ea
	ii_lset_opea	\13,d			// (ax), (ax)+
	ii_lset_opea	\13,e			// -(ax),d16(ax)
	ii_lset_opeag	\13,f			// d8(ax,dy)
	lea	table+0x\13\2f8*4,a0
	move.l	#ii_0x\13f8,(a0)+		// xxx.w
	move.l	#ii_0x\13f9,(a0)+		// xxx.l
// roxr.w ea
	ii_lset_opea	\14,d			// (ax), (ax)+
	ii_lset_opea	\14,e			// -(ax),d16(ax)
	ii_lset_opeag	\14,f			// d8(ax,dy)
	lea	table+0x\14\2f8*4,a0
	move.l	#ii_0x\14f8,(a0)+		// xxx.w
	move.l	#ii_0x\14f9,(a0)+		// xxx.l
// roxl.w ea
	ii_lset_opea	\15,e			// (ax), (ax)+
	ii_lset_opea	\15,e			// -(ax),d16(ax)
	ii_lset_opeag	\15,f			// d8(ax,dy)
	lea	table+0x\15\2f8*4,a0
	move.l	#ii_0x\15f8,(a0)+		// xxx.w
	move.l	#ii_0x\15f9,(a0)+		// xxx.l
// ror.w ea
	ii_lset_opea	\16,d			// (ax), (ax)+
	ii_lset_opea	\16,e			// -(ax),d16(ax)
	ii_lset_opeag	\16,f			// d8(ax,dy)
	lea	table+0x\16\2f8*4,a0
	move.l	#ii_0x\16f8,(a0)+		// xxx.w
	move.l	#ii_0x\16f9,(a0)+		// xxx.l
// rol.w ea
	ii_lset_opea	\17,d			// (ax), (ax)+
	ii_lset_opea	\17,e			// -(ax),d16(ax)
	ii_lset_opeag	\17,f			// d8(ax,dy)
	lea	table+0x\17\2f8*4,a0
	move.l	#ii_0x\17f8,(a0)+		// xxx.w
	move.l	#ii_0x\17f9,(a0)+		// xxx.l
// ende
.endm;	
/******************************************************/
ii_shift_op:.macro	// 1=code 
//byt-------------------------------
//asx.b	#x,dx
	ii_shift_op2agb	0,as,a
//lsx.b	#x,dx
	ii_shift_op2aub	0,ls,a
//roxx.b	#x,dx
	ii_shift_op2agb	1,rox,a
//rox.b	#x,dx
	ii_shift_op2aub	1,ro,a
//asx.b	dy,dx
	ii_shift_op2agb	2,as,b
//lsx.b	dy,dx
	ii_shift_op2aub	2,ls,b
//roxx.b dy,dx
	ii_shift_op2agb	3,rox,b
//rox.b	dy,dx
	ii_shift_op2aub	3,ro,b
// word ---------------------------------------
//asx.w	#x,dx
	ii_shift_op2agw	4,as,a
//lsx.w	#x,dx
	ii_shift_op2auw	4,ls,a
//roxx.w	#x,dx
	ii_shift_op2agw	5,rox,a
//rox.w	#x,dx
	ii_shift_op2auw	5,ro,a
//asx.w	dy,dx
	ii_shift_op2agw	6,as,b
//lsx.w	dy,dx
	ii_shift_op2auw	6,ls,b
//roxx.w dy,dx
	ii_shift_op2agw	7,rox,b
//rox.w	dy,dx
	ii_shift_op2auw	7,ro,b
// long ---------------------------------------
//roxx.l #x,dx
	ii_shift_op2agw	9,rox,a
//rox.l	#x,dx
	ii_shift_op2auw	9,ro,a
//roxx.l dy,dx
	ii_shift_op2agw	b,rox,b
//rox.l	dy,dx
	ii_shift_op2auw	b,ro,b
// ea ---------------------------------------
//asr.w	#1,ea
	ii_shift_op2ea	0,asr
//asl.w	#1,ea
	ii_shift_op2ea	1,asl
//lsr.w	#1,ea
	ii_shift_op2ea	2,lsr,
//lsl.w	#1,ea
	ii_shift_op2ea	3,lsl
//roxr.w #1,ea
	ii_shift_op2ea	4,roxr
//roxl.w #1,ea
	ii_shift_op2ea	5,roxl
//ror.w	#1,ea
	ii_shift_op2ea	6,ror
//rol.w	#1,ea
	ii_shift_op2ea	7,rol
.endm
//byt ============================================
ii_shift_op2agb:.macro	//byt: 1=code 2=operation 3=quick(a) oder register(b)
	ii_shift_op1\3b	\1,0,\2,d0_off+3(a7)
	ii_shift_op1\3b	\1,1,\2,d1_off+3(a7)
	ii_shift_op1\3b	\1,2,\2,d2
	ii_shift_op1\3b	\1,3,\2,d3
	ii_shift_op1\3b	\1,4,\2,d4
	ii_shift_op1\3b	\1,5,\2,d5
	ii_shift_op1\3b	\1,6,\2,d6
	ii_shift_op1\3b	\1,7,\2,d7
.endm

ii_shift_op2aub:.macro	//byt: 1=code 2=operation
	ii_shift_op1\3b	\1,8,\2,d0_off+3(a7)
	ii_shift_op1\3b	\1,9,\2,d1_off+3(a7)
	ii_shift_op1\3b	\1,a,\2,d2
	ii_shift_op1\3b	\1,b,\2,d3
	ii_shift_op1\3b	\1,c,\2,d4
	ii_shift_op1\3b	\1,d,\2,d5
	ii_shift_op1\3b	\1,e,\2,d6
	ii_shift_op1\3b	\1,f,\2,d7
.endm

ii_shift_op1ab:.macro	// z.B. asr.w. #x,dx 1=code 3.ziffer 2=code 4.ziffer 3=shift art 4=dx
	ii_shift_op0	0\1\2,b,\3r,#8,\4
	ii_shift_op0	2\1\2,b,\3r,#1,\4
	ii_shift_op0	4\1\2,b,\3r,#2,\4
	ii_shift_op0	6\1\2,b,\3r,#3,\4
	ii_shift_op0	8\1\2,b,\3r,#4,\4
	ii_shift_op0	a\1\2,b,\3r,#5,\4
	ii_shift_op0	c\1\2,b,\3r,#6,\4
	ii_shift_op0	e\1\2,b,\3r,#7,\4
	ii_shift_op0	1\1\2,b,\3l,#8,\4
	ii_shift_op0	3\1\2,b,\3l,#1,\4
	ii_shift_op0	5\1\2,b,\3l,#2,\4
	ii_shift_op0	7\1\2,b,\3l,#3,\4
	ii_shift_op0	9\1\2,b,\3l,#4,\4
	ii_shift_op0	b\1\2,b,\3l,#5,\4
	ii_shift_op0	d\1\2,b,\3l,#6,\4
	ii_shift_op0	f\1\2,b,\3l,#7,\4
.endm
	
ii_shift_op1bb:.macro	// z.B. asr.w. #x,dx 1=code 3.ziffer 2=code 4.ziffer 3=shift art 4=dx
	ii_shift_op0b	0\1\2,b,\3r,d0_off(a7),\4
	ii_shift_op0b	2\1\2,b,\3r,d1_off(a7),\4
	ii_shift_op0	4\1\2,b,\3r,d2,\4
	ii_shift_op0	6\1\2,b,\3r,d3,\4
	ii_shift_op0	8\1\2,b,\3r,d4,\4
	ii_shift_op0	a\1\2,b,\3r,d5,\4
	ii_shift_op0	c\1\2,b,\3r,d6,\4
	ii_shift_op0	e\1\2,b,\3r,d7,\4
	ii_shift_op0b	1\1\2,b,\3l,d0_off(a7),\4
	ii_shift_op0b	3\1\2,b,\3l,d1_off(a7),\4
	ii_shift_op0	5\1\2,b,\3l,d2,\4
	ii_shift_op0	7\1\2,b,\3l,d3,\4
	ii_shift_op0	9\1\2,b,\3l,d4,\4
	ii_shift_op0	b\1\2,b,\3l,d5,\4
	ii_shift_op0	d\1\2,b,\3l,d6,\4
	ii_shift_op0	f\1\2,b,\3l,d7,\4
.endm
// word ---------------------------------------
ii_shift_op2agw:.macro	//byt: 1=code 2=operation 3=quick(a) oder register(b)
	ii_shift_op1\3w	\1,0,\2,d0_off+2(a7)
	ii_shift_op1\3w	\1,1,\2,d1_off+2(a7)
	ii_shift_op1\3w	\1,2,\2,d2
	ii_shift_op1\3w	\1,3,\2,d3
	ii_shift_op1\3w	\1,4,\2,d4
	ii_shift_op1\3w	\1,5,\2,d5
	ii_shift_op1\3w	\1,6,\2,d6
	ii_shift_op1\3w	\1,7,\2,d7
.endm

ii_shift_op2auw:.macro	//byt: 1=code 2=operation
	ii_shift_op1\3w	\1,8,\2,d0_off+2(a7)
	ii_shift_op1\3w	\1,9,\2,d1_off+2(a7)
	ii_shift_op1\3w	\1,a,\2,d2
	ii_shift_op1\3w	\1,b,\2,d3
	ii_shift_op1\3w	\1,c,\2,d4
	ii_shift_op1\3w	\1,d,\2,d5
	ii_shift_op1\3w	\1,e,\2,d6
	ii_shift_op1\3w	\1,f,\2,d7
.endm

ii_shift_op1aw:.macro	// z.B. asr.w. #x,dx 1=code 3.ziffer 2=code 4.ziffer 3=shift art 4=dx
	ii_shift_op0	0\1\2,w,\3r,#8,\4
	ii_shift_op0	2\1\2,w,\3r,#1,\4
	ii_shift_op0	4\1\2,w,\3r,#2,\4
	ii_shift_op0	6\1\2,w,\3r,#3,\4
	ii_shift_op0	8\1\2,w,\3r,#4,\4
	ii_shift_op0	a\1\2,w,\3r,#5,\4
	ii_shift_op0	c\1\2,w,\3r,#6,\4
	ii_shift_op0	e\1\2,w,\3r,#7,\4
	ii_shift_op0	1\1\2,w,\3l,#8,\4
	ii_shift_op0	3\1\2,w,\3l,#1,\4
	ii_shift_op0	5\1\2,w,\3l,#2,\4
	ii_shift_op0	7\1\2,w,\3l,#3,\4
	ii_shift_op0	9\1\2,w,\3l,#4,\4
	ii_shift_op0	b\1\2,w,\3l,#5,\4
	ii_shift_op0	d\1\2,w,\3l,#6,\4
	ii_shift_op0	f\1\2,w,\3l,#7,\4
.endm
	
ii_shift_op1bw:.macro	// z.B. asr.w. #x,dx 1=code 3.ziffer 2=code 4.ziffer 3=shift art 4=dx
	ii_shift_op0b	0\1\2,w,\3r,d0_off(a7),\4
	ii_shift_op0b	2\1\2,w,\3r,d1_off(a7),\4
	ii_shift_op0	4\1\2,w,\3r,d2,\4
	ii_shift_op0	6\1\2,w,\3r,d3,\4
	ii_shift_op0	8\1\2,w,\3r,d4,\4
	ii_shift_op0	a\1\2,w,\3r,d5,\4
	ii_shift_op0	c\1\2,w,\3r,d6,\4
	ii_shift_op0	e\1\2,w,\3r,d7,\4
	ii_shift_op0b	1\1\2,w,\3l,d0_off(a7),\4
	ii_shift_op0b	3\1\2,w,\3l,d1_off(a7),\4
	ii_shift_op0	5\1\2,w,\3l,d2,\4
	ii_shift_op0	7\1\2,w,\3l,d3,\4
	ii_shift_op0	9\1\2,w,\3l,d4,\4
	ii_shift_op0	b\1\2,w,\3l,d5,\4
	ii_shift_op0	d\1\2,w,\3l,d6,\4
	ii_shift_op0	f\1\2,w,\3l,d7,\4
.endm
// long ---------------------------------------
ii_shift_op2agl:.macro	//byt: 1=code 2=operation 3=quick(a) oder register(b)
	ii_shift_op1\3l	\1,0,\2,d0_off(a7)
	ii_shift_op1\3l	\1,1,\2,d1_off(a7)
	ii_shift_op1\3l	\1,2,\2,d2
	ii_shift_op1\3l	\1,3,\2,d3
	ii_shift_op1\3l	\1,4,\2,d4
	ii_shift_op1\3l	\1,5,\2,d5
	ii_shift_op1\3l	\1,6,\2,d6
	ii_shift_op1\3l	\1,7,\2,d7
.endm

ii_shift_op2aul:.macro	//byt: 1=code 2=operation
	ii_shift_op1\3l	\1,8,\2,d0_off(a7)
	ii_shift_op1\3l	\1,9,\2,d1_off(a7)
	ii_shift_op1\3l	\1,a,\2,d2
	ii_shift_op1\3l	\1,b,\2,d3
	ii_shift_op1\3l	\1,c,\2,d4
	ii_shift_op1\3l	\1,d,\2,d5
	ii_shift_op1\3l	\1,e,\2,d6
	ii_shift_op1\3l	\1,f,\2,d7
.endm

ii_shift_op1al:.macro	// z.B. asr.w. #x,dx 1=code 3.ziffer 2=code 4.ziffer 3=shift art 4=dx
	ii_shift_op0	0\1\2,l,\3r,#8,\4
	ii_shift_op0	2\1\2,l,\3r,#1,\4
	ii_shift_op0	4\1\2,l,\3r,#2,\4
	ii_shift_op0	6\1\2,l,\3r,#3,\4
	ii_shift_op0	8\1\2,l,\3r,#4,\4
	ii_shift_op0	a\1\2,l,\3r,#5,\4
	ii_shift_op0	c\1\2,l,\3r,#6,\4
	ii_shift_op0	e\1\2,l,\3r,#7,\4
	ii_shift_op0	1\1\2,l,\3l,#8,\4
	ii_shift_op0	3\1\2,l,\3l,#1,\4
	ii_shift_op0	5\1\2,l,\3l,#2,\4
	ii_shift_op0	7\1\2,l,\3l,#3,\4
	ii_shift_op0	9\1\2,l,\3l,#4,\4
	ii_shift_op0	b\1\2,l,\3l,#5,\4
	ii_shift_op0	d\1\2,l,\3l,#6,\4
	ii_shift_op0	f\1\2,l,\3l,#7,\4
.endm
	
ii_shift_op1bl:.macro	// z.B. asr.w. #x,dx 1=code 3.ziffer 2=code 4.ziffer 3=shift art 4=dx
	ii_shift_op0b	0\1\2,l,\3r,d0_off(a7),\4
	ii_shift_op0b	2\1\2,l,\3r,d1_off(a7),\4
	ii_shift_op0	4\1\2,l,\3r,d2,\4
	ii_shift_op0	6\1\2,l,\3r,d3,\4
	ii_shift_op0	8\1\2,l,\3r,d4,\4
	ii_shift_op0	a\1\2,l,\3r,d5,\4
	ii_shift_op0	c\1\2,l,\3r,d6,\4
	ii_shift_op0	e\1\2,l,\3r,d7,\4
	ii_shift_op0b	1\1\2,l,\3l,d0_off(a7),\4
	ii_shift_op0b	3\1\2,l,\3l,d1_off(a7),\4
	ii_shift_op0	5\1\2,l,\3l,d2,\4
	ii_shift_op0	7\1\2,l,\3l,d3,\4
	ii_shift_op0	9\1\2,l,\3l,d4,\4
	ii_shift_op0	b\1\2,l,\3l,d5,\4
	ii_shift_op0	d\1\2,l,\3l,d6,\4
	ii_shift_op0	f\1\2,l,\3l,d7,\4
.endm
// .word ea ============================================
ii_shift_op2ea:.macro		//1=code 2.ziffer 2=shiftart
// (a0) bis (a7) ----------------------------
ii_0xe\1d0:				 
	move.l	a0_off(a7),a1
	ii_shift_typ	w,\2,#1,(a1),(a1).
ii_0xe\1d1:				 
	move.l	a1_off(a7),a1
	ii_shift_typ	w,\2,#1,(a1),(a1).
ii_0xe\1d2:	
	ii_shift_typ	w,\2,#1,(a2),(a2).
ii_0xe\1d3:	
	ii_shift_typ	w,\2,#1,(a3),(a3).
ii_0xe\1d4:	
	ii_shift_typ	w,\2,#1,(a4),(a4).
ii_0xe\1d5:	
	ii_shift_typ	w,\2,#1,(a5),(a5).
ii_0xe\1d6:	
	ii_shift_typ	w,\2,#1,(a6),(a6).
ii_0xe\1d7:	
	move.l	usp,a1
	ii_shift_typ	w,\2,#1,(a1),(a1).
// (a0)+ bis (a7)+ -----------------------------
ii_0xe\1d8:				 
	move.l	a0_off(a7),a1
	addq.l	#2,a0_off(a7)
	ii_shift_typ	w,\2,#1,(a1),(a1).
ii_0xe\1d9:				 
	move.l	a1_off(a7),a1
	addq.l	#2,a0_off(a7)
	ii_shift_typ	w,\2,#1,(a1),(a1).
ii_0xe\1da:	
	ii_shift_typ	w,\2,#1,(a2),(a2)+.
ii_0xe\1db:	
	ii_shift_typ	w,\2,#1,(a3),(a3)+
ii_0xe\1dc:	
	ii_shift_typ	w,\2,#1,(a4),(a4)+
ii_0xe\1dd:	
	ii_shift_typ	w,\2,#1,(a5),(a5)+
ii_0xe\1de:	
	ii_shift_typ	w,\2,#1,(a6),(a6)+
ii_0xe\1df:	
	move.l	usp,a1
	addq.l	#2,a1
	move.l	a1,usp
	subq.l	#2,a1
	ii_shift_typ	w,\2,#1,(a1),(a1).
// -(a0) bis -(a7) -----------------------------
ii_0xe\1e0:				 
	move.l	a0_off(a7),a1
	subq.l	#2,a1
	move.l	a1,a0_off(a7)
	ii_shift_typ	w,\2,#1,(a1),(a1).
ii_0xe\1e1:				 
	move.l	a1_off(a7),a1
	subq.l	#2,a1
	move.l	a1,a1_off(a7)
	ii_shift_typ	w,\2,#1,(a1),(a1).
ii_0xe\1e2:	
	ii_shift_typ	w,\2,#1,-(a2),(a2).
ii_0xe\1e3:	
	ii_shift_typ	w,\2,#1,-(a3),(a3)
ii_0xe\1e4:	
	ii_shift_typ	w,\2,#1,-(a4),(a4)
ii_0xe\1e5:	
	ii_shift_typ	w,\2,#1,-(a5),(a5)
ii_0xe\1e6:	
	ii_shift_typ	w,\2,#1,-(a6),(a6)
ii_0xe\1e7:	
	move.l	usp,a1
	subq.l	#2,a1
	move.l	a1,usp
	ii_shift_typ	w,\2,#1,(a1),(a1).
// d16(a0) bis d16(a7) -----------------------------
ii_0xe\1e8:	
	move.w	(a0)+,a1			 
	add.l	a0_off(a7),a1
	ii_shift_typ	w,\2,#1,(a1),(a1).
ii_0xe\1e9:				 
	move.w	(a0)+,a1			 
	add.l	a1_off(a7),a1
	ii_shift_typ	w,\2,#1,(a1),(a1).
ii_0xe\1ea:	
	move.w	(a0)+,a1			 
	add.l	a2,a1
	ii_shift_typ	w,\2,#1,(a1),(a1).
ii_0xe\1eb:	
	move.w	(a0)+,a1			 
	add.l	a3,a1
	ii_shift_typ	w,\2,#1,(a1),(a1)
ii_0xe\1ec:	
	move.w	(a0)+,a1			 
	add.l	a4,a1
	ii_shift_typ	w,\2,#1,(a1),(a1)
ii_0xe\1ed:	
	move.w	(a0)+,a1			 
	add.l	a5,a1
	ii_shift_typ	w,\2,#1,(a1),(a1)
ii_0xe\1ee:	
	move.w	(a0)+,a1			 
	add.l	a6,a1
	ii_shift_typ	w,\2,#1,(a1),(a1)
ii_0xe\1ef:	
	mvs.w	(a0)+,d0
	move.l	usp,a1			 
	add.l	d0,a1
	ii_shift_typ	w,\2,#1,(a1),(a1).
// d8(a0,dy) bis d8(a7,dy) -----------------------------
ii_0xe\1f0:	
	move.l	a0_off(a0),a1			 
	jsr	ewf
	ii_shift_typ	w,\2,#1,(a1),(a1).
ii_0xe\1f1:				 
	move.l	a1_off(a0),a1			 
	jsr	ewf
	ii_shift_typ	w,\2,#1,(a1),(a1).
ii_0xe\1f2:	
	move.l	a2,a1			 
	jsr	ewf
	ii_shift_typ	w,\2,#1,(a1),(a1).
ii_0xe\1f3:	
	move.l	a3,a1			 
	jsr	ewf
	ii_shift_typ	w,\2,#1,(a1),(a1).
ii_0xe\1f4:	
	move.l	a4,a1			 
	jsr	ewf
	ii_shift_typ	w,\2,#1,(a1),(a1).
ii_0xe\1f5:	
	move.l	a5,a1			 
	jsr	ewf
	ii_shift_typ	w,\2,#1,(a1),(a1).
ii_0xe\1f6:	
	move.l	a6,a1			 
	jsr	ewf
	ii_shift_typ	w,\2,#1,(a1),(a1).
ii_0xe\1f7:	
	move.l	usp,a1			 
	jsr	ewf
	ii_shift_typ	w,\2,#1,(a1),(a1).
// xxx.w xxx.l
ii_0xe\1f8:	
	move.w	(a0)+,a1			 
	ii_shift_typ	w,\2,#1,(a1),(a1).
ii_0xe\1f9:				 
	move.l	(a0)+,a1			 
	ii_shift_typ	w,\2,#1,(a1),(a1).
.endm
//============================================================================
//subroutine
//------------------------------
ii_shift_op0:.macro	// shift: 1=code 2=size 3=shift art 4=shift wert 5=ea
ii_0xe\1:
	ii_shift_typ	\2,\3,\4,\5,\5
.endm	

ii_shift_op0b:.macro	// shift wert nach d0 holen: 1=code 2=size 3=shift art 4=shift wert 5=ea
ii_0xe\1:
	move.l	\4,d0
	ii_shift_typ	\2,\3,d0,\5,\5
.endm	

ii_shift_typ:.macro	//1=size 2=shift art 3=shift wert 4=source 5=dest
#ifdef	halten
	halt
#endif
.ifc	asr,\2
	mvs.\1	\4,d1
.else
	mvz.\1	\4,d1
.endif
.ifc	roxr,\2
	nop
.else
	.ifc	roxl,\2
		nop
	.else
		.ifc	ror,\2
			nop
		.else
			.ifc	rol,\2
				nop
			.else
				\2.l	\3,d1
			.endif
		.endif
	.endif
.endif
	set_cc0
	move.\1	d1,\5
	ii_end
.endm
