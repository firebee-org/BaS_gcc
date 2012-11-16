//-------------------------------------------------------------------
// move
//-------------------------------------------------------------------
.extern		ewf

.text
ii_move_lset:.macro
//-------------------------------------------------------------------------
// 0x1000 move.b
//-------------------------------------------------------------------------
// move.x	d16(ax),xxx.w 1=size 2=adress register 
	ii_lset_opeau	11,e
// move.x   d16(ax),xxx.l
	ii_lset_opeau	13,e
// move.x	d16(pc),xxx.w
	ii_lset	0x11fa
// move.x	d16(pc),xxx.l
	ii_lset	0x13fa
// move.x	xxx.w,d16(ax)
	ii_lset_dxu	1,78
// move.x	xxx.l,d16(ax)
	ii_lset_dxu	1,79
// move.x	#xx,d16(ax)
	ii_lset_dxu	1,7c
// move.x	xxx.w,xxx.w; 1=size 2=size source adr 3=size dest adr 4=code
	ii_lset	0x11f8
// move.x	xxx.l,xxx.w
	ii_lset	0x11f9
// move.x	xxx.w,xxx.l
	ii_lset	0x13f8
// move.x	xxx.l,xxx.l
	ii_lset	0x13f9
// move.x	#xx,xxx.w 	//1=size 2=size dest adr 3=code
	ii_lset	0x11fc
// move.x	#xx,xxx.l 	//1=size 2=size dest adr 3=code
	ii_lset	0x13fc
//---------------------------------------------------------------------
// move.x	ea,d8(ax,dy) -------------------------------------------
//---------------------------------------------------------------------
// move.x	dx,d8(a0-a7,dy)
	ii_lset_opeag	11,8
	ii_lset_opeag	13,8
	ii_lset_opeag	15,8
	ii_lset_opeag	17,8
	ii_lset_opeag	19,8
	ii_lset_opeag	1b,8
	ii_lset_opeag	1d,8
	ii_lset_opeag	1f,8
// move.x	(ax),d8(a0-a7,dy)
// move.x	(ax)+,d8(a0-a7,dy)
	ii_lset_opea	11,9
	ii_lset_opea	13,9
	ii_lset_opea	15,9
	ii_lset_opea	17,9
	ii_lset_opea	19,9
	ii_lset_opea	1b,9
	ii_lset_opea	1d,9
	ii_lset_opea	1f,9
// move.x	-(ax),d8(a0-a7,dy)
	ii_lset_opeag	11,a
	ii_lset_opeag	13,a
	ii_lset_opeag	15,a
	ii_lset_opeag	17,a
	ii_lset_opeag	19,a
	ii_lset_opeag	1b,a
	ii_lset_opeag	1d,a
	ii_lset_opeag	1f,a
//---------------------------------------------------------------------
// move.x	ea,d8(pc,dy) -------------------------------------------
//---------------------------------------------------------------------
// move.x	dx,d8(pc,dy)
	ii_lset_opeag	17,c
// move.x	(ax),d8(pc,dy)
// move.x	(ax)+,d8(pc,dy)
	ii_lset_opea	17,d
// move.x	-(ax),d8(pc,dy)
	ii_lset_opeag	17,e
//---------------------------------------------------------------------
// move.x	d8(ax,dy),ea --------------------------------------------
//---------------------------------------------------------------------
// move.x	d8(ax,dy),d0-d7
	ii_lset_opeag	10,3
	ii_lset_opeag	12,3
	ii_lset_opeag	14,3
	ii_lset_opeag	16,3
	ii_lset_opeag	18,3
	ii_lset_opeag	1a,3
	ii_lset_opeag	1c,3
	ii_lset_opeag	1e,3
// move.x	d8(ax,dy),a0-a7
	ii_lset_opeag	10,7
	ii_lset_opeag	12,7
	ii_lset_opeag	14,7
	ii_lset_opeag	16,7
	ii_lset_opeag	18,7
	ii_lset_opeag	1a,7
	ii_lset_opeag	1c,7
	ii_lset_opeag	1e,7
// move.x	d8(ax,dy),(a0)-(a7)
	ii_lset_opeag	10,b
	ii_lset_opeag	12,b
	ii_lset_opeag	14,b
	ii_lset_opeag	16,b
	ii_lset_opeag	18,b
	ii_lset_opeag	1a,b
	ii_lset_opeag	1c,b
	ii_lset_opeag	1e,b
// move.x	d8(ax,dy),(a0)+-(a7)+
	ii_lset_opeag	10,f
	ii_lset_opeag	12,f
	ii_lset_opeag	14,f
	ii_lset_opeag	16,f
	ii_lset_opeag	18,f
	ii_lset_opeag	1a,f
	ii_lset_opeag	1c,f
	ii_lset_opeag	1e,f
// move.x	d8(ax,dy),-(a0) bis -(a7)
	ii_lset_opeag	11,3
	ii_lset_opeag	13,3
	ii_lset_opeag	15,3
	ii_lset_opeag	17,3
	ii_lset_opeag	19,3
	ii_lset_opeag	1b,3
	ii_lset_opeag	1d,3
	ii_lset_opeag	1f,3
//---------------------------------------------------------------------
// move.x	d8(pc,dy),ea --------------------------------------------
//---------------------------------------------------------------------
// move.x	d8(pc,dy),d0-d7
	ii_lset_dxg		1,3b
// move.x	d8(pc,dy),a0-a7
	ii_lset_dxg		1,7b
// move.x	d8(pc,dy),(a0-a7)
	ii_lset_dxg		1,bb
// move.x	d8(pc,dy),(a0-a7)+
	ii_lset_dxg		1,fb
// move.x	d8(pc,dy),(a0-a7)+
	ii_lset_dxu		1,3b
//-------------------------------------------------------------------------
// 0x2000 move.l
//-------------------------------------------------------------------------
// move.x	d16(ax),xxx.w 1=size 2=adress register 
	ii_lset_opeau	21,e
// move.x   d16(ax),xxx.l
	ii_lset_opeau	23,e
// move.x	d16(pc),xxx.w
	ii_lset	0x21fa
// move.x	d16(pc),xxx.l
	ii_lset	0x23fa
// move.x	xxx.w,d16(ax)
	ii_lset_dxu	2,78
// move.x	xxx.l,d16(ax)
	ii_lset_dxu	2,79
// move.x	#xx,d16(ax)
	ii_lset_dxu	2,7c
// move.x	xxx.w,xxx.w; 1=size 2=size source adr 3=size dest adr 4=code
	ii_lset	0x21f8
// move.x	xxx.l,xxx.w
	ii_lset	0x21f9
// move.x	xxx.w,xxx.l
	ii_lset	0x23f8
// move.x	xxx.l,xxx.l
	ii_lset	0x23f9
// move.x	#xx,xxx.w 	//1=size 2=size dest adr 3=code
	ii_lset	0x21fc
// move.x	#xx,xxx.l 	//1=size 2=size dest adr 3=code
	ii_lset	0x23fc
// move.x	ea,d8(ax,dy) -------------------------------------------
// move.x	dx,d8(a0-a7,dy)
	ii_lset_opeag	21,8
	ii_lset_opeag	23,8
	ii_lset_opeag	25,8
	ii_lset_opeag	27,8
	ii_lset_opeag	29,8
	ii_lset_opeag	2b,8
	ii_lset_opeag	2d,8
	ii_lset_opeag	2f,8
// move.x	(ax),d8(a0-a7,dy)
// move.x	(ax)+,d8(a0-a7,dy)
	ii_lset_opea	21,9
	ii_lset_opea	23,9
	ii_lset_opea	25,9
	ii_lset_opea	27,9
	ii_lset_opea	29,9
	ii_lset_opea	2b,9
	ii_lset_opea	2d,9
	ii_lset_opea	2f,9
// move.x	-(ax),d8(a0-a7,dy)
	ii_lset_opeag	21,a
	ii_lset_opeag	23,a
	ii_lset_opeag	25,a
	ii_lset_opeag	27,a
	ii_lset_opeag	29,a
	ii_lset_opeag	2b,a
	ii_lset_opeag	2d,a
	ii_lset_opeag	2f,a
//---------------------------------------------------------------------
// move.x	ea,d8(pc,dy) -------------------------------------------
//---------------------------------------------------------------------
// move.x	dx,d8(pc,dy)
// move.x	ax,d8(pc,dy)
	ii_lset_opea	27,c
// move.x	(ax),d8(pc,dy)
// move.x	(ax)+,d8(pc,dy)
	ii_lset_opea	27,d
// move.x	-(ax),d8(pc,dy)
	ii_lset_opeag	27,e
//---------------------------------------------------------------------
// move.x	d8(ax,dy),ea --------------------------------------------
//---------------------------------------------------------------------
// move.x	d8(ax,dy),d0-d7
	ii_lset_opeag	20,3
	ii_lset_opeag	22,3
	ii_lset_opeag	24,3
	ii_lset_opeag	26,3
	ii_lset_opeag	28,3
	ii_lset_opeag	2a,3
	ii_lset_opeag	2c,3
	ii_lset_opeag	2e,3
// move.x	d8(ax,dy),a0-a7
	ii_lset_opeag	20,7
	ii_lset_opeag	22,7
	ii_lset_opeag	24,7
	ii_lset_opeag	26,7
	ii_lset_opeag	28,7
	ii_lset_opeag	2a,7
	ii_lset_opeag	2c,7
	ii_lset_opeag	2e,7
// move.x	d8(ax,dy),(a0)-(a7)
	ii_lset_opeag	20,b
	ii_lset_opeag	22,b
	ii_lset_opeag	24,b
	ii_lset_opeag	26,b
	ii_lset_opeag	28,b
	ii_lset_opeag	2a,b
	ii_lset_opeag	2c,b
	ii_lset_opeag	2e,b
// move.x	d8(ax,dy),(a0)+-(a7)+
	ii_lset_opeag	20,f
	ii_lset_opeag	22,f
	ii_lset_opeag	24,f
	ii_lset_opeag	26,f
	ii_lset_opeag	28,f
	ii_lset_opeag	2a,f
	ii_lset_opeag	2c,f
	ii_lset_opeag	2e,f
// move.x	d8(ax,dy),-(a0) bis -(a7)
	ii_lset_opeag	21,3
	ii_lset_opeag	23,3
	ii_lset_opeag	25,3
	ii_lset_opeag	27,3
	ii_lset_opeag	29,3
	ii_lset_opeag	2b,3
	ii_lset_opeag	2d,3
	ii_lset_opeag	2f,3
//---------------------------------------------------------------------
// move.x	d8(pc,dy),ea --------------------------------------------
//---------------------------------------------------------------------
// move.x	d8(pc,dy),d0-d7
	ii_lset_dxg		2,3b
// move.x	d8(pc,dy),a0-a7
	ii_lset_dxg		2,7b
// move.x	d8(pc,dy),(a0-a7)
	ii_lset_dxg		2,bb
// move.x	d8(pc,dy),(a0-a7)+
	ii_lset_dxg		2,fb
// move.x	d8(pc,dy),(a0-a7)+
	ii_lset_dxu		2,3b
//-------------------------------------------------------------------------
// 0x3000 move.w
///-------------------------------------------------------------------------
// move.x	d16(ax),xxx.w 1=size 2=adress register 
	ii_lset_opeau	31,e
// move.x   d16(ax),xxx.l
	ii_lset_opeau	33,e
// move.x	d16(pc),xxx.w
	ii_lset	0x31fa
// move.x	d16(pc),xxx.l
	ii_lset	0x33fa
// move.x	xxx.w,d16(ax)
	ii_lset_dxu	3,78
// move.x	xxx.l,d16(ax)
	ii_lset_dxu	3,79
// move.x	#xx,d16(ax)
	ii_lset_dxu	3,7c
// move.x	xxx.w,xxx.w; 1=size 2=size source adr 3=size dest adr 4=code
	ii_lset	0x31f8
// move.x	xxx.l,xxx.w
	ii_lset	0x31f9
// move.x	xxx.w,xxx.l
	ii_lset	0x33f8
// move.x	xxx.l,xxx.l
	ii_lset	0x33f9
// move.x	#xx,xxx.w 	//1=size 2=size dest adr 3=code
	ii_lset	0x31fc
// move.x	#xx,xxx.l 	//1=size 2=size dest adr 3=code
	ii_lset	0x33fc
// move.x	ea,d8(ax,dy) -------------------------------------------
// move.x	dx,d8(a0-a7,dy)
	ii_lset_opeag	31,8
	ii_lset_opeag	33,8
	ii_lset_opeag	35,8
	ii_lset_opeag	37,8
	ii_lset_opeag	39,8
	ii_lset_opeag	3b,8
	ii_lset_opeag	3d,8
	ii_lset_opeag	3f,8
// move.x	(ax),d8(a0-a7,dy)
// move.x	(ax)+,d8(a0-a7,dy)
	ii_lset_opea	31,9
	ii_lset_opea	33,9
	ii_lset_opea	35,9
	ii_lset_opea	37,9
	ii_lset_opea	39,9
	ii_lset_opea	3b,9
	ii_lset_opea	3d,9
	ii_lset_opea	3f,9
// move.x	-(ax),d8(a0-a7,dy)
	ii_lset_opeag	31,a
	ii_lset_opeag	33,a
	ii_lset_opeag	35,a
	ii_lset_opeag	37,a
	ii_lset_opeag	39,a
	ii_lset_opeag	3b,a
	ii_lset_opeag	3d,a
	ii_lset_opeag	3f,a
//---------------------------------------------------------------------
// move.x	ea,d8(pc,dy) -------------------------------------------
//---------------------------------------------------------------------
// move.x	dx,d8(pc,dy)
// move.x	ax,d8(pc,dy)
	ii_lset_opea	37,c
// move.x	(ax),d8(pc,dy)
// move.x	(ax)+,d8(pc,dy)
	ii_lset_opea	37,d
// move.x	-(ax),d8(pc,dy)
	ii_lset_opeag	37,e
//---------------------------------------------------------------------
// move.x	d8(ax,dy),ea --------------------------------------------
//---------------------------------------------------------------------
// move.x	d8(ax,dy),d0-d7
	ii_lset_opeag	30,3
	ii_lset_opeag	32,3
	ii_lset_opeag	34,3
	ii_lset_opeag	36,3
	ii_lset_opeag	38,3
	ii_lset_opeag	3a,3
	ii_lset_opeag	3c,3
	ii_lset_opeag	3e,3
// move.x	d8(ax,dy),a0-a7
	ii_lset_opeag	30,7
	ii_lset_opeag	32,7
	ii_lset_opeag	34,7
	ii_lset_opeag	36,7
	ii_lset_opeag	38,7
	ii_lset_opeag	3a,7
	ii_lset_opeag	3c,7
	ii_lset_opeag	3e,7
// move.x	d8(ax,dy),(a0)-(a7)
	ii_lset_opeag	30,b
	ii_lset_opeag	32,b
	ii_lset_opeag	34,b
	ii_lset_opeag	36,b
	ii_lset_opeag	38,b
	ii_lset_opeag	3a,b
	ii_lset_opeag	3c,b
	ii_lset_opeag	3e,b
// move.x	d8(ax,dy),(a0)+-(a7)+
	ii_lset_opeag	30,f
	ii_lset_opeag	32,f
	ii_lset_opeag	34,f
	ii_lset_opeag	36,f
	ii_lset_opeag	38,f
	ii_lset_opeag	3a,f
	ii_lset_opeag	3c,f
	ii_lset_opeag	3e,f
// move.x	d8(ax,dy),-(a0) bis -(a7)
	ii_lset_opeag	31,3
	ii_lset_opeag	33,3
	ii_lset_opeag	35,3
	ii_lset_opeag	37,3
	ii_lset_opeag	39,3
	ii_lset_opeag	3b,3
	ii_lset_opeag	3d,3
	ii_lset_opeag	3f,3
//---------------------------------------------------------------------
// move.x	d8(pc,dy),ea --------------------------------------------
//---------------------------------------------------------------------
// move.x	d8(pc,dy),d0-d7
	ii_lset_dxg		3,3b
// move.x	d8(pc,dy),a0-a7
	ii_lset_dxg		3,7b
// move.x	d8(pc,dy),(a0-a7)
	ii_lset_dxg		3,bb
// move.x	d8(pc,dy),(a0-a7)+
	ii_lset_dxg		3,fb
// move.x	d8(pc,dy),(a0-a7)+
	ii_lset_dxu		3,3b
.endm
//---------------------------------------------------------------------------------------------
// function
//---------------------------------------------------------------------------------------------
ii_move_op:.macro
// move.x	d16(ax),xxx.w 1=size 2=adress register 
	ii_move_d16ax_xxx	1e8,a0_off(a7),w
	ii_move_d16ax_xxx	1e9,a1_off(a7),w
	ii_move_d16ax_xxx	1ea,a2,w
	ii_move_d16ax_xxx	1eb,a3,w
	ii_move_d16ax_xxx	1ec,a4,w
	ii_move_d16ax_xxx	1ed,a5,w
	ii_move_d16ax_xxx	1ee,a6,w
	ii_move_d16ax_xxx	1ef,usp,w
// move.x   d16(ax),xxx.l
	ii_move_d16ax_xxx	3e8,a0_off(a7),l
	ii_move_d16ax_xxx	3e9,a1_off(a7),l
	ii_move_d16ax_xxx	3ea,a2,l
	ii_move_d16ax_xxx	3eb,a3,l
	ii_move_d16ax_xxx	3ec,a4,l
	ii_move_d16ax_xxx	3ed,a5,l
	ii_move_d16ax_xxx	3ee,a6,l
	ii_move_d16ax_xxx	3ef,usp,l
// move.x	d16(pc),xxx.w
	ii_move_d16ax_xxx	1fa,a0,w
// move.x	d16(pc),xxx.l
	ii_move_d16ax_xxx	3fa,a0,l
// move.x	xxx.w,d16(ax)
// move.x	xxx.l,d16(ax)
// move.x	#xx,d16(ax)
	ii_move_xxx_d16ax	1,a0_off(a7)
	ii_move_xxx_d16ax	3,a1_off(a7)
	ii_move_xxx_d16ax	5,a2
	ii_move_xxx_d16ax	7,a3
	ii_move_xxx_d16ax	9,a4
	ii_move_xxx_d16ax	b,a5
	ii_move_xxx_d16ax	d,a6
	ii_move_xxx_d16ax	f,usp
// move.x	xxx.w,xxx.w; 1=size 2=size source adr 3=size dest adr 4=code
	ii_move_xxx_xxx		b,w,w,11f8
	ii_move_xxx_xxx		l,w,w,21f8
	ii_move_xxx_xxx		w,w,w,31f8
// move.x	xxx.l,xxx.w
	ii_move_xxx_xxx		b,l,w,11f9
	ii_move_xxx_xxx		l,l,w,21f9
	ii_move_xxx_xxx		w,l,w,31f9
// move.x	xxx.w,xxx.l
	ii_move_xxx_xxx		b,w,l,13f8
	ii_move_xxx_xxx		l,w,l,23f8
	ii_move_xxx_xxx		w,w,l,33f8
// move.x	xxx.l,xxx.l
	ii_move_xxx_xxx		b,l,l,13f9
	ii_move_xxx_xxx		l,l,l,23f9
	ii_move_xxx_xxx		w,l,l,33f9
// move.x	#xx,xxx.w 	//1=size 2=size dest adr 3=code
	ii_move_im_xxx		b,w,11fc
	ii_move_im_xxx		l,w,21fc
	ii_move_im_xxx		w,w,31fc
// move.x	#xx,xxx.l 	//1=size 2=size dest adr 3=code
	ii_move_im_xxx		b,l,13fc
	ii_move_im_xxx		l,l,23fc
	ii_move_im_xxx		w,l,33fc
//---------------------------------------------------------------------
// move.x	ea,d8(ax,dy) -------------------------------------------
//---------------------------------------------------------------------
// move.x	dx,d8(ax/pc,dy)
	ii_move_dxxia	d0_off(a7),80,id,d,c0
	ii_move_dxxia	d1_off(a7),81,id,d,c1
	ii_move_dxxia	d2,82,d,d,c2
	ii_move_dxxia	d3,83,d,d,c3
	ii_move_dxxia	d4,84,d,d,c4
	ii_move_dxxia	d5,85,d,d,c5
	ii_move_dxxia	d6,86,d,d,c6
	ii_move_dxxia	d7,87,d,d,c7
// move.x	ax,d8(ax/pc,dy)
	ii_move_dxxia	a0_off(a7),88,id,d,c8
	ii_move_dxxia	a1_off(a7),89,id,d,c9
	ii_move_dxxia	a2,8a,d,da,ca
	ii_move_dxxia	a3,8b,d,da,cb
	ii_move_dxxia	a4,8c,d,da,cc
	ii_move_dxxia	a5,8d,d,da,cd
	ii_move_dxxia	a6,8e,d,da,ce
	ii_move_dxxia	a7,8f,a7,da,cf
// move.x	(ax),d8(ax/pc,dy)
	ii_move_dxxia	a0_off(a7),90,ia,d,d0
	ii_move_dxxia	a1_off(a7),91,ia,d,d1
	ii_move_dxxia	(a2),92,d,d,d2
	ii_move_dxxia	(a3),93,d,d,d3
	ii_move_dxxia	(a4),94,d,d,d4
	ii_move_dxxia	(a5),95,d,d,d5
	ii_move_dxxia	(a6),96,d,d,d6
	ii_move_dxxia	(a7),97,a7,d,d7
// move.x	(ax)+,d8(ax/pc,dy)
	ii_move_dxxia	a0_off(a7),98,iap,d,d8
	ii_move_dxxia	a1_off(a7),99,iap,d,d9
	ii_move_dxxia	(a2)+,9a,d,d,da
	ii_move_dxxia	(a3)+,9b,d,d,db
	ii_move_dxxia	(a4)+,9c,d,d,dc
	ii_move_dxxia	(a5)+,9d,d,d,dd
	ii_move_dxxia	(a6)+,9e,d,d,de
	ii_move_dxxia	(a7)+,9f,a7,d,df
// move.x	-(ax),d8(ax/pc,dy)
	ii_move_dxxia	a0_off(a7),a0,iam,d,e0
	ii_move_dxxia	a1_off(a7),a1,iam,d,e1
	ii_move_dxxia	-(a2),a2,d,d,e2
	ii_move_dxxia	-(a3),a3,d,d,e3
	ii_move_dxxia	-(a4),a4,d,d,e4
	ii_move_dxxia	-(a5),a5,d,d,e5
	ii_move_dxxia	-(a6),a6,d,d,e6
	ii_move_dxxia	-(a7),a7,a7,d,e7
//---------------------------------------------------------------------
// move.x	d8(ax/pc,dy),ea --------------------------------------------
//---------------------------------------------------------------------
// move.x	d8(ax/pc,dy),dz
	ii_move_d8_dest	d0,id,03,dx	
	ii_move_d8_dest	d1,id,23,dx	
	ii_move_d8_dest	d2,d,43,dx	
	ii_move_d8_dest	d3,d,63,dx	
	ii_move_d8_dest	d4,d,83,dx	
	ii_move_d8_dest	d5,d,a3,dx
	ii_move_d8_dest	d6,d,c3,dx
	ii_move_d8_dest	d7,d,e3,dx
// move.x	d8(ax/pc,dy),az
	ii_move_d8_dest	a0,id,07,ax	
	ii_move_d8_dest	a1,id,27,ax	
	ii_move_d8_dest	a2,d,47,ax	
	ii_move_d8_dest	a3,d,67,ax	
	ii_move_d8_dest	a4,d,87,ax	
	ii_move_d8_dest	a5,d,a7,ax	
	ii_move_d8_dest	a6,d,c7,ax	
	ii_move_d8_dest	usp,id,e7,ax	
// move.x	d8(ax/pc,dy),(az)
	ii_move_d8_dest	a0_off(a7),id,0b,ia	
	ii_move_d8_dest	a1_off(a7),id,2b,ia	
	ii_move_d8_dest	(a2),d,4b,ia	
	ii_move_d8_dest	(a3),d,6b,ia	
	ii_move_d8_dest	(a4),d,8b,ia	
	ii_move_d8_dest	(a5),d,ab,ia	
	ii_move_d8_dest	(a6),d,cb,ia	
	ii_move_d8_dest	usp,id,eb,ia	
// move.x	d8(ax/pc,dy),(az)+
	ii_move_d8_dest	a0_off(a7),id,0f,iap	
	ii_move_d8_dest	a1_off(a7),id,2f,iap	
	ii_move_d8_dest	(a2)+,d,4f,iap	
	ii_move_d8_dest	(a3)+,d,6f,iap	
	ii_move_d8_dest	(a4)+,d,8f,iap	
	ii_move_d8_dest	(a5)+,d,af,iap	
	ii_move_d8_dest	(a6)+,d,cf,iap	
	ii_move_d8_dest	usp,id,ef,iap	
// move.x	d8(ax/pc,dy),-(az)
	ii_move_d8_dest	a0_off(a7),id,13,iam	
	ii_move_d8_dest	a1_off(a7),id,33,iam	
	ii_move_d8_dest	-(a2),d,53,iam	
	ii_move_d8_dest	-(a3),d,73,iam	
	ii_move_d8_dest	-(a4),d,93,iam	
	ii_move_d8_dest	-(a5),d,b3,iam	
	ii_move_d8_dest	-(a6),d,d3,iam	
	ii_move_d8_dest	usp,id,f3,iam	
.endm //end function
//====================================================================
// subs ----------------------------------------------------------
//====================================================================
// move.x	d16(ax),xxx.w/l 1=code 2=adress register 3=dest adr size
ii_move_d16ax_xxx:.macro
ii_0x1\1:			//byt:
#ifdef	halten_move
	halt
#endif
	mvs.w	(a0)+,d0
	move.l	\2,a1
	add.l	d0,a1
	move.b	(a1),d0
	move.\3	(a0)+,a1
	move.b	d0,(a1)
	set_cc0
	ii_end
ii_0x2\1:			//long:
#ifdef	halten_move
	halt
#endif
	mvs.w	(a0)+,d0
	move.l	\2,a1
	add.l	d0,a1
	move.l	(a1),d0
	move.\3	(a0)+,a1
	move.l	d0,(a1)
	set_cc0
	ii_end
ii_0x3\1:			//word:
#ifdef	halten_move
	halt
#endif
	mvs.w	(a0)+,d0
	move.l	\2,a1
	add.l	d0,a1
	move.w	(a1),d0
	move.\3	(a0)+,a1
	move.w	d0,(a1)
	set_cc0
	ii_end
.endm
//----------------------------------------
// move.x	xxx.w,d16(ax)
// move.x	xxx.l,d16(ax)
// move.x	#xx,d16(ax)
ii_move_xxx_d16ax:.macro	//1=code 2=adress register
ii_0x1\178:			//byt xxx.w
#ifdef	halten_move
	halt
#endif
	move.w	(a0)+,a1
	move.b	(a1),d0
	move.l	\2,a1
	mvs.w	(a0)+,d1
	add.l	d1,a1
	move.b	d0,(a1)
	set_cc0
	ii_end
ii_0x1\179:			//byt xxx.l
#ifdef	halten_move
	halt
#endif
	move.l	(a0)+,a1
	move.b	(a1),d0
	move.l	\2,a1
	mvs.w	(a0)+,d1
	add.l	d1,a1
	move.b	d0,(a1)
	set_cc0
	ii_end
ii_0x1\17c:			//byt #x
#ifdef	halten_move
	halt
#endif
	mvs.b	(a0)+,d0
	move.l	\2,a1
	mvs.w	(a0)+,d1
	add.l	d1,a1
	move.b	d0,(a1)
	set_cc0
	ii_end
ii_0x2\178:			//long xxx.w
#ifdef	halten_move
	halt
#endif
	move.w	(a0)+,a1
	move.l	(a1),d0
	move.l	\2,a1
	mvs.w	(a0)+,d1
	add.l	d1,a1
	move.l	d0,(a1)
	set_cc0
	ii_end
ii_0x2\179:			//long xxx.l
#ifdef	halten_move
	halt
#endif
	move.l	(a0)+,a1
	move.l	(a1),d0
	move.l	\2,a1
	mvs.w	(a0)+,d1
	add.l	d1,a1
	move.l	d0,(a1)
	set_cc0
	ii_end
ii_0x2\17c:			//long #x
#ifdef	halten_move
	halt
#endif
	move.l	(a0)+,d0
	move.l	\2,a1
	mvs.w	(a0)+,d1
	add.l	d1,a1
	move.l	d0,(a1)
	set_cc0
	ii_end
	ii_end
ii_0x3\178:			//word xxx.w
#ifdef	halten_move
	halt
#endif
	move.w	(a0)+,a1
	move.w	(a1),d0
	move.l	\2,a1
	mvs.w	(a0)+,d1
	add.l	d1,a1
	move.w	d0,(a1)
	set_cc0
	ii_end
ii_0x3\179:			//word xxx.l
#ifdef	halten_move
	halt
#endif
	move.l	(a0)+,a1
	move.w	(a1),d0
	move.l	\2,a1
	mvs.w	(a0)+,d1
	add.l	d1,a1
	move.w	d0,(a1)
	set_cc0
	ii_end
ii_0x3\17c:			//word #x
#ifdef	halten_move
	halt
#endif
	move.w	(a0)+,d0
	move.l	\2,a1
	mvs.w	(a0)+,d1
	add.l	d1,a1
	move.w	d0,(a1)
	set_cc0
	ii_end
.endm
// move.x	xxx,xxx
ii_move_xxx_xxx:.macro	//1=size 2=size source adr 3=size dest adr 4=code
ii_0x\4:	
#ifdef	halten_move
	halt
#endif
	move.\2	(a0)+,a1
	move.\1	(a1),d0
	move.\3	(a0)+,d1
	move.\1	d0,(a1)
	set_cc0
	ii_end
.endm
// move.x	im,xxx
ii_move_im_xxx:.macro	//1=size 2=size dest adr 3=code
ii_0x\3:	
#ifdef	halten_move
	halt
#endif
.ifc	1,b
	move.w	(a0)+,d0
.else
	move.\1	(a0)+,d0
.endif
	move.\2	(a0)+,a1
	move.\1	d0,(a1)
	set_cc0
	ii_end
.endm
//---------------------------------------------------------------------
// move.x	ea,d8(ax,dy) -------------------------------------------
//---------------------------------------------------------------------
// ea=dx,ax,(ax),(ax)+,-(ax) 
//---------------------------------------------------------------------
ii_move_dxxia:.macro  //1=source 2=code 1.stelle 3=code 2 letzte Stellen 4=art 5=code d8(pc,dy)	
.ifc	\3,id 
	ii_move_dxxi	b,\1+3,1,\2,\3,\5
	ii_move_dxxi	w,\1+2,2,\2,\3,\5
	ii_move_dxxi	l,\1,3,\2,\3,\5
.else
	.ifc	\4,da 
		ii_move_dxxi	w,\1,2,\2,\3,\5
		ii_move_dxxi	l,\1,3,\2,\3,\5
	.else
 		ii_move_dxxi	b,\1,1,\2,\3,\5
		ii_move_dxxi	w,\1,2,\2,\3,\5
		ii_move_dxxi	l,\1,3,\2,\3,\5
	.endif
.endif
.endm 

ii_move_dxxi:.macro
ii_0x\31\4:	//1=size 2=source 3=code 1.stelle 4=code 2 letzte Stellen
#ifdef	halten_move
	halt
#endif
	move.l	a0_off(a7),a1
	move_end	\1,\2,\5
ii_0x\33\4:	//1=size 2=source 3=code 1.stelle 4=code 2 letzte Stellen
#ifdef	halten_move
	halt
#endif
	move.l	a1_off(a7),a1
	move_end	\1,\2,\5
ii_0x\35\4:	//1=size 2=source 3=code 1.stelle 4=code 2 letzte Stellen
#ifdef	halten_move
	halt
#endif
	move.l	a2,a1
	move_end	\1,\2,\5
ii_0x\37\4:	//1=size 2=source 3=code 1.stelle 4=code 2 letzte Stellen
#ifdef	halten_move
	halt
#endif
	move.l	a3,a1
	move_end	\1,\2,\5
ii_0x\39\4:	//1=size 2=source 3=code 1.stelle 4=code 2 letzte Stellen
#ifdef	halten_move
	halt
#endif
	move.l	a4,a1
	move_end	\1,\2,\5
ii_0x\3b\4:	//1=size 2=source 3=code 1.stelle 4=code 2 letzte Stellen
#ifdef	halten_move
	halt
#endif
	move.l	a5,a1
	move_end	\1,\2,\5
ii_0x\3d\4:	//1=size 2=source 3=code 1.stelle 4=code 2 letzte Stellen
#ifdef	halten_move
	halt
#endif
	move.l	a6,a1
	move_end	\1,\2,\5
ii_0x\3f\4:	//1=size 2=source 3=code 1.stelle 4=code 2 letzte Stellen
#ifdef	halten_move
	halt
#endif
	move.l	usp,a1
	move_end	\1,\2,\5
ii_0x\37\6:	//1=size 2=source 3=code 1.stelle 4=code 2 letzte Stellen
#ifdef	halten_move
	halt
#endif
	move.l	a0,a1
	move_end	\1,\2,\5
.endm
//------------------------------------
move_end:.macro
	jsr		ewf
.ifc	3,a7
	move.l	a7,d1		// a7 sichern
	move.l 	usp,a7		// a7 holen
.endif
.ifc	3,ia
	move.l	a2,d1		// a2 sichern
	move.l	\2,a2
	move.\2	(a2),(a1)
	move.l	d1,a2		// a2 zurück
.else
	.ifc	3,iap
		move.l	a2,d1		// a2 sichern
		move.l	\2,a2
		move.\2	(a2)+,(a1)
	.else
		.ifc	3,iam
			move.l	a2,d1		// a2 sichern
			move.l	\2,a2
			move.\2	-(a2),(a1)
		.else
			move.\1	\2,(a1)
		.endif
	.endif
.endif
.ifc	3,a7
	movea.l	a7,usp		// a7 zurück
	movea.l d1,a7		// a7 setzen
.endif
	set_cc0
.ifc	3,iap
	move.l	d1,a2		// a2 zurück
.endif
.ifc	3,iam
	move.l	d1,a2		// a2 zurück
.endif
	ii_end
.endm
//---------------------------------------------------------------------
// move.x 	ea,d8(pc,dy)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// move.x	d8(ax,dy),ea ---------------------------------------------
//---------------------------------------------------------------------
ii_move_d8_dest:.macro	//1=dest 2=art 3=code 2.+3.stelle 4=art adresse
//byt
ii_0x1\30:	
#ifdef	halten_move
	halt
#endif
			move.l	a0_off(a7),a1
			moveb_a1_src\4	\1,\2
ii_0x1\31:	
#ifdef	halten_move
	halt
#endif
			move.l	a1_off(a7),a1
			moveb_a1_src\4	\1,\2
ii_0x1\32:	
#ifdef	halten_move
	halt
#endif
			move.l	a2,a1
			moveb_a1_src\4	\1,\2
ii_0x1\33:	
#ifdef	halten_move
	halt
#endif
			move.l	a3,a1
			moveb_a1_src\4	\1,\2
ii_0x1\34:	
#ifdef	halten_move
	halt
#endif
			move.l	a4,a1
			moveb_a1_src\4	\1,\2
ii_0x1\35:	
#ifdef	halten_move
	halt
#endif
			move.l	a5,a1
			moveb_a1_src\4	\1,\2
ii_0x1\36:	
#ifdef	halten_move
	halt
#endif
			move.l	a6,a1
			moveb_a1_src\4	\1,\2
ii_0x1\37:	
#ifdef	halten_move
	halt
#endif
			move.l	usp,a1
			moveb_a1_src\4	\1,\2
ii_0x1\3b:	
#ifdef	halten_move
	halt
#endif
			move.l	a0,a1
			moveb_a1_src\4	\1,\2
//long
ii_0x2\30:	
#ifdef	halten_move
	halt
#endif
			move.l	a0_off(a7),a1
			movel_a1_src\4	\1,\2
ii_0x2\31:	
#ifdef	halten_move
	halt
#endif
			move.l	a1_off(a7),a1
			movel_a1_src\4	\1,\2
ii_0x2\32:	
#ifdef	halten_move
	halt
#endif
			move.l	a2,a1
			movel_a1_src\4	\1,\2
ii_0x2\33:	
#ifdef	halten_move
	halt
#endif
			move.l	a3,a1
			movel_a1_src\4	\1,\2
ii_0x2\34:	
#ifdef	halten_move
	halt
#endif
			move.l	a4,a1
			movel_a1_src\4	\1,\2
ii_0x2\35:	
#ifdef	halten_move
	halt
#endif
			move.l	a5,a1
			movel_a1_src\4	\1,\2
ii_0x2\36:	
#ifdef	halten_move
	halt
#endif
			move.l	a6,a1
			movel_a1_src\4	\1,\2
ii_0x2\37:	
#ifdef	halten_move
	halt
#endif
			move.l	usp,a1
			movel_a1_src\4	\1,\2
ii_0x2\3b:	
#ifdef	halten_move
	halt
#endif
			move.l	a0,a1
			moveb_a1_src\4	\1,\2
//word
ii_0x3\30:	
#ifdef	halten_move
	halt
#endif
			move.l	a0_off(a7),a1
			movew_a1_src\4	\1,\2
ii_0x3\31:	
#ifdef	halten_move
	halt
#endif
			move.l	a1_off(a7),a1
			movew_a1_src\4	\1,\2
ii_0x3\32:	
#ifdef	halten_move
	halt
#endif
			move.l	a2,a1
			movew_a1_src\4	\1,\2
ii_0x3\33:	
#ifdef	halten_move
	halt
#endif
			move.l	a3,a1
			movew_a1_src\4	\1,\2
ii_0x3\34:	
#ifdef	halten_move
	halt
#endif
			move.l	a4,a1
			movew_a1_src\4	\1,\2
ii_0x3\35:	
#ifdef	halten_move
	halt
#endif
			move.l	a5,a1
			movew_a1_src\4	\1,\2
ii_0x3\36:	
#ifdef	halten_move
	halt
#endif
			move.l	a6,a1
			movew_a1_src\4	\1,\2
ii_0x3\37:	
#ifdef	halten_move
	halt
#endif
			move.l	usp,a1
			movew_a1_src\4	\1,\2
ii_0x3\3b:	
#ifdef	halten_move
	halt
#endif
			move.l	a0,a1
			moveb_a1_src\4	\1,\2
.endm
//---------------------------------------------------------------------
//dx
moveb_a1_srcdx:.macro
			jsr		ewf
.ifc	\2,id
			move.b	(a1),\1_off+3(a7)
.else
			move.b	(a1),\1
.endif
			set_cc0
			ii_end
.endm
movel_a1_srcdx:.macro
			jsr		ewf
.ifc	\2,id
			move.l	(a1),\1_off(a7)
.else
			move.l	(a1),\1
.endif
			set_cc0
			ii_end
.endm
movew_a1_srcdx:.macro
			jsr		ewf
.ifc	\2,id
			move.w	(a1),\1_off+2(a7)
.else
			move.w	(a1),\1
.endif
			set_cc0
			ii_end
.endm
// ax
moveb_a1_srcax:.macro
			jsr		ewf
.ifc	\2,id
	.ifc	\1,usp
			move.w	(a1),a1
			move.l	a1,usp
	.else
			move.w	(a1),\1_off+2(a7)
	.endif
.else
			move.w	(a1),\1
.endif
			set_cc0
			ii_end
.endm
movel_a1_srcax:.macro
			jsr		ewf
.ifc	\2,id
	.ifc	\1,usp
			move.l	(a1),a1
			move.l	a1,usp
	.else
			move.l	(a1),\1_off(a7)
	.endif
.else
			move.l	(a1),\1
.endif
			set_cc0
			ii_end
.endm
movew_a1_srcax:.macro
			jsr		ewf
.ifc	\2,id
	.ifc	\1,usp
			move.w	(a1),a1
			move.l	a1,usp
	.else
			move.w	(a1),\1_off+2(a7)
	.endif
.else
			move.w	(a1),\1
.endif
			set_cc0
			ii_end
.endm
// (ax)
moveb_a1_srcia:.macro
			jsr		ewf
.ifc	\2,id
			move.b	(a1),d0
			move.l	\1,a1
			move.b	d0,(a1)
.else
			move.b	(a1),\1
.endif
			set_cc0
			ii_end
.endm
movel_a1_srcia:.macro
			jsr		ewf
.ifc	\2,id
			move.l	(a1),d0
			move.l	\1,a1
			move.l	d0,(a1)
.else
			move.l	(a1),\1
.endif
			set_cc0
			ii_end
.endm
movew_a1_srcia:.macro
			jsr		ewf
.ifc	\2,id
			move.w	(a1),d0
			move.l	\1,a1
			move.w	d0,(a1)
.else
			move.w	(a1),\1
.endif
			set_cc0
			ii_end
.endm
// (ax)+
moveb_a1_srciap:.macro
			jsr		ewf
.ifc	\2,id
			move.b	(a1),d0
			move.l	\1,a1
			move.b	d0,(a1)+
			move.l	a1,\1
.else
			move.b	(a1),\1
.endif
			set_cc0
			ii_end
.endm
movel_a1_srciap:.macro
			jsr		ewf
.ifc	\2,id
			move.l	(a1),d0
			move.l	\1,a1
			move.l	d0,(a1)+
			move.l	a1,\1
.else
			move.l	(a1),\1
.endif
			set_cc0
			ii_end
.endm
movew_a1_srciap:.macro
			jsr		ewf
.ifc	\2,id
			move.w	(a1),d0
			move.l	\1,a1
			move.w	d0,(a1)+
			move.l	a1,\1
.else
			move.w	(a1),\1
.endif
			set_cc0
			ii_end
.endm
// -(ax)
moveb_a1_srciam:.macro
			jsr		ewf
.ifc	\2,id
			move.b	(a1),d0
			move.l	\1,a1
			move.b	d0,-(a1)
			move.l	a1,\1
.else
			move.b	(a1),\1
.endif
			set_cc0
			ii_end
.endm
movel_a1_srciam:.macro
			jsr		ewf
.ifc	\2,id
			move.l	(a1),d0
			move.l	\1,a1
			move.l	d0,-(a1)
			move.l	a1,\1
.else
			move.l	(a1),\1
.endif
			set_cc0
			ii_end
.endm
movew_a1_srciam:.macro
			jsr		ewf
.ifc	\2,id
			move.w	(a1),d0
			move.l	\1,a1
			move.w	d0,-(a1)
			move.l	a1,\1
.else
			move.w	(a1),\1
.endif
			set_cc0
			ii_end
.endm

//---------------------------------------------------------------------
// move.x	d8(pc,dy),ea
//---------------------------------------------------------------------


 
// move.w dx,(a0,dx.w*SF)
	ii_lset		0x3180
	ii_lset		0x3181
	ii_lset		0x3182
	ii_lset		0x3183
	ii_lset		0x3184
	ii_lset		0x3185
	ii_lset		0x3186
	ii_lset		0x3187 

//--------------------------------------------------------------------
// // move.w	dx,d(ay,dz.w*sf)
//--------------------------------------------------------------------
movew_ewfw:.macro off1, off2
	move.l		\off2,a1
	jsr			ewf
	move.w		\off1,(a1)
	set_cc0
	ii_end
	.endm
ii_0x3180:
	movew_ewfw 	d0_off+2(a7),a0_off(a7)
ii_0x3181:
	movew_ewfw 	d0_off+2(a7),a0_off(a7)
ii_0x3182:
	movew_ewfw 	d2,a0_off(a7)
ii_0x3183:
	movew_ewfw 	d3,a0_off(a7)
ii_0x3184:
	movew_ewfw 	d4,a0_off(a7)
ii_0x3185:
	movew_ewfw 	d5,a0_off(a7)
ii_0x3186:
	movew_ewfw 	d6,a0_off(a7)
ii_0x3187:
	movew_ewfw 	d7,a0_off(a7)
