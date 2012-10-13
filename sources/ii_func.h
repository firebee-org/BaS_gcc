//--------------------------------------------------------------------
// functionen macros
//--------------------------------------------------------------------
ii_lset_func:.macro
/******************************************************/
// byt
/******************************************************/
// func.b dy,dx
	ii_lset_dx	\1,00						// 0x1.22 -> z.B. 1=d2=4 ->0xd07c -> 0xde7c			
	ii_lset_dx	\1,01			
	ii_lset_dx	\1,02			
	ii_lset_dx	\1,03			
	ii_lset_dx	\1,04			
	ii_lset_dx	\1,05			
	ii_lset_dx	\1,06			
	ii_lset_dx	\1,07			
// func.b ax,dx
	ii_lset_dxu	\1,08			
	ii_lset_dxu	\1,09			
	ii_lset_dxu	\1,0a			
	ii_lset_dxu	\1,0b			
	ii_lset_dxu	\1,0c			
	ii_lset_dxu	\1,0d			
	ii_lset_dxu	\1,0e			
	ii_lset_dxu	\1,0f			
// func.b (ax),dx
	ii_lset_dx	\1,10			
	ii_lset_dx	\1,11			
	ii_lset_dx	\1,12			
	ii_lset_dx	\1,13			
	ii_lset_dx	\1,14			
	ii_lset_dx	\1,15			
	ii_lset_dx	\1,16			
	ii_lset_dx	\1,17			
// func.b (ax)+,dx
	ii_lset_dx	\1,18			
	ii_lset_dx	\1,19			
	ii_lset_dx	\1,1a			
	ii_lset_dx	\1,1b			
	ii_lset_dx	\1,1c			
	ii_lset_dx	\1,1d			
	ii_lset_dx	\1,1e			
	ii_lset_dx	\1,1f			
// func.b -(ax),dx
	ii_lset_dx	\1,20			
	ii_lset_dx	\1,21			
	ii_lset_dx	\1,22			
	ii_lset_dx	\1,23			
	ii_lset_dx	\1,24			
	ii_lset_dx	\1,25			
	ii_lset_dx	\1,26			
	ii_lset_dx	\1,27			
// func.b d16(ax),dx
	ii_lset_dx	\1,28			
	ii_lset_dx	\1,29			
	ii_lset_dx	\1,2a			
	ii_lset_dx	\1,2b			
	ii_lset_dx	\1,2c			
	ii_lset_dx	\1,2d			
	ii_lset_dx	\1,2e			
	ii_lset_dx	\1,2f			
// func.b dd8(ax,dy),dx
	ii_lset_dx	\1,30			
	ii_lset_dx	\1,31			
	ii_lset_dx	\1,32			
	ii_lset_dx	\1,33			
	ii_lset_dx	\1,34			
	ii_lset_dx	\1,35			
	ii_lset_dx	\1,36			
	ii_lset_dx	\1,37			
// func.b xxx.w,dx
	ii_lset_dx	\1,38						// 0x1.22 -> z.B. 1=d2=4 ->0xd07c -> 0xde7c
// func.b xxx.l,dx
	ii_lset_dx	\1,39						// 0x1.22 -> z.B. 1=d2=4 ->0xd07c -> 0xde7c
// func.b d16(pc),dx
	ii_lset_dxg	\1,7a						// 0x1.22 -> z.B. 1=d2=4 ->0xd07c -> 0xde7c
// func.b d8(pc,dy),dx
	ii_lset_dxg	\1,3b						// 0x1.22 -> z.B. 1=d2=4 ->0xd07c -> 0xde7c
// func.b #im,dx
	ii_lset_dxg	\1,3c						// 0x1.22 -> z.B. 1=d2=4 ->0xd07c -> 0xde7c
/******************************************************/
// word
/******************************************************/
// func.w dy,dx
	ii_lset_dx	\1,40						// 0x1.22 -> z.B. 1=d2=4 ->0xd07c -> 0xde7c			
	ii_lset_dx	\1,41			
	ii_lset_dx	\1,42			
	ii_lset_dx	\1,43			
	ii_lset_dx	\1,44			
	ii_lset_dx	\1,45			
	ii_lset_dx	\1,46			
	ii_lset_dx	\1,47			
// func.w ax,dx
	ii_lset_dx	\1,48			
	ii_lset_dx	\1,49			
	ii_lset_dx	\1,4a			
	ii_lset_dx	\1,4b			
	ii_lset_dx	\1,4c			
	ii_lset_dx	\1,4d			
	ii_lset_dx	\1,4e			
	ii_lset_dx	\1,4f			
// func.w (ax),dx
	ii_lset_dx	\1,50			
	ii_lset_dx	\1,51			
	ii_lset_dx	\1,52			
	ii_lset_dx	\1,53			
	ii_lset_dx	\1,54			
	ii_lset_dx	\1,55			
	ii_lset_dx	\1,56			
	ii_lset_dx	\1,57			
// func.w (ax)+,dx
	ii_lset_dx	\1,58			
	ii_lset_dx	\1,59			
	ii_lset_dx	\1,5a			
	ii_lset_dx	\1,5b			
	ii_lset_dx	\1,5c			
	ii_lset_dx	\1,5d			
	ii_lset_dx	\1,5e			
	ii_lset_dx	\1,5f			
// func.w -(ax),dx
	ii_lset_dx	\1,60			
	ii_lset_dx	\1,61			
	ii_lset_dx	\1,62			
	ii_lset_dx	\1,63			
	ii_lset_dx	\1,64			
	ii_lset_dx	\1,65			
	ii_lset_dx	\1,66			
	ii_lset_dx	\1,67			
// func.w d16(ax),dx
	ii_lset_dx	\1,68			
	ii_lset_dx	\1,69			
	ii_lset_dx	\1,6a			
	ii_lset_dx	\1,6b			
	ii_lset_dx	\1,6c			
	ii_lset_dx	\1,6d			
	ii_lset_dx	\1,6e			
	ii_lset_dx	\1,6f			
// func.w d8(ax,dy),dx
	ii_lset_dx	\1,70			
	ii_lset_dx	\1,71			
	ii_lset_dx	\1,72			
	ii_lset_dx	\1,73			
	ii_lset_dx	\1,74			
	ii_lset_dx	\1,75			
	ii_lset_dx	\1,76			
	ii_lset_dx	\1,77			
// func.w xxx.w,dx
	ii_lset_dx	\1,78						// 0x1.22 -> z.B. 1=d2=4 ->0xd07c -> 0xde7c
// func.w xxx.l,dx
	ii_lset_dx	\1,79						// 0x1.22 -> z.B. 1=d2=4 ->0xd07c -> 0xde7c
// func.w d16(pc),dx
	ii_lset_dxg	\1,7a						// 0x1.22 -> z.B. 1=d2=4 ->0xd07c -> 0xde7c
// func.w d8(pc,dy),dx
	ii_lset_dxg	\1,7b						// 0x1.22 -> z.B. 1=d2=4 ->0xd07c -> 0xde7c
// func.w #im,dx
	ii_lset_dxg	\1,7c						// 0x1.22 -> z.B. 1=d2=4 ->0xd07c -> 0xde7c
/******************************************************/
// long
/******************************************************/
// func.l ax,dx		-> -(ay),-(ax)
	ii_lset_dxu	\1,c8			
	ii_lset_dxu	\1,c9			
	ii_lset_dxu	\1,ca			
	ii_lset_dxu	\1,cb			
	ii_lset_dxu	\1,cc			
	ii_lset_dxu	\1,cd			
	ii_lset_dxu	\1,ce			
	ii_lset_dxu	\1,cf			
// func.w d8(ax,dy),dx
	ii_lset_dx	\1,b0			
	ii_lset_dx	\1,b1			
	ii_lset_dx	\1,b2			
	ii_lset_dx	\1,b3			
	ii_lset_dx	\1,b4			
	ii_lset_dx	\1,b5			
	ii_lset_dx	\1,b6			
	ii_lset_dx	\1,b7			
// func.l d8(pc,dy),dx
	ii_lset_dxg	\1,bb						// 0x1.22 -> z.B. 1=d2=4 ->0xd07c -> 0xde7c
/******************************************************/
// adress register
/******************************************************/
//func.w dy,ax
	ii_lset_dxg	\1,c0			
	ii_lset_dxg	\1,c1			
	ii_lset_dxg	\1,c2			
	ii_lset_dxg	\1,c3			
	ii_lset_dxg	\1,c4			
	ii_lset_dxg	\1,c5			
	ii_lset_dxg	\1,c6			
	ii_lset_dxg	\1,c7			
//func.w ay,ax
	ii_lset_dxg	\1,c8			
	ii_lset_dxg	\1,c9			
	ii_lset_dxg	\1,ca			
	ii_lset_dxg	\1,cb			
	ii_lset_dxg	\1,cc			
	ii_lset_dxg	\1,cd			
	ii_lset_dxg	\1,ce			
	ii_lset_dxg	\1,cf			
//func.w (ay),ax
	ii_lset_dxg	\1,d0			
	ii_lset_dxg	\1,d1			
	ii_lset_dxg	\1,d2			
	ii_lset_dxg	\1,d3			
	ii_lset_dxg	\1,d4			
	ii_lset_dxg	\1,d5			
	ii_lset_dxg	\1,d6			
	ii_lset_dxg	\1,d7			
//func.w (ay)+,ax
	ii_lset_dxg	\1,d8			
	ii_lset_dxg	\1,d9			
	ii_lset_dxg	\1,da			
	ii_lset_dxg	\1,db			
	ii_lset_dxg	\1,dc			
	ii_lset_dxg	\1,dd			
	ii_lset_dxg	\1,de			
	ii_lset_dxg	\1,df			
//func.w -(ay),ax
	ii_lset_dxg	\1,e0			
	ii_lset_dxg	\1,e1			
	ii_lset_dxg	\1,e2			
	ii_lset_dxg	\1,e3			
	ii_lset_dxg	\1,e4			
	ii_lset_dxg	\1,e5			
	ii_lset_dxg	\1,e6			
	ii_lset_dxg	\1,e7			
//func.w d16(ay),ax
	ii_lset_dxg	\1,e8			
	ii_lset_dxg	\1,e9			
	ii_lset_dxg	\1,ea			
	ii_lset_dxg	\1,eb			
	ii_lset_dxg	\1,ec			
	ii_lset_dxg	\1,ed			
	ii_lset_dxg	\1,ee			
	ii_lset_dxg	\1,ef			
//func.w d8(ay,dy),ax
	ii_lset_dxg	\1,f0			
	ii_lset_dxg	\1,f1			
	ii_lset_dxg	\1,f2			
	ii_lset_dxg	\1,f3			
	ii_lset_dxg	\1,f4			
	ii_lset_dxg	\1,f5			
	ii_lset_dxg	\1,f6			
	ii_lset_dxg	\1,f7			
// func.w xxx.w,ax
	ii_lset_dxg	\1,f8
// func.w xxx.l,ax
	ii_lset_dxg	\1,f9
// func.w d16(pc),ax
	ii_lset_dxg	\1,fa
// func.w d8(pc,dy),ax
	ii_lset_dxg	\1,fb
// func.w #im,ax
	ii_lset_dxg	\1,fc
//--------------------------------------------------------------------
// ende
	.endm;	
/*****************************************************************************************/
ii_func:.macro
//--------------------------------------------------------------------
// byt
//--------------------------------------------------------------------
///--------------------------------------------------------------------
//  func.b	ds,dx
//--------------------------------------------------------------------
	funcbeadx	\1,00,\2dd,d0_off+3(a7)		
	funcbeadx	\1,01,\2dd,d1_off+3(a7)		
	funcbeadx	\1,02,\2dd,d2		
	funcbeadx	\1,03,\2dd,d3		
	funcbeadx	\1,04,\2dd,d4		
	funcbeadx	\1,05,\2dd,d5		
	funcbeadx	\1,06,\2dd,d6		
	funcbeadx	\1,07,\2dd,d7
//--------------------------------------------------------------------
//  func.b	(ax),dx
//--------------------------------------------------------------------
	funcbeadx	\1,10,\2dda,a0_off(a7)		
	funcbeadx	\1,11,\2dda,a1_off(a7)		
	funcbeadx	\1,12,\2dd,(a2)		
	funcbeadx	\1,13,\2dd,(a3)		
	funcbeadx	\1,14,\2dd,(a4)		
	funcbeadx	\1,15,\2dd,(a5)		
	funcbeadx	\1,16,\2dd,(a6)		
	funcbeadx	\1,17,\2dda,usp
//--------------------------------------------------------------------
//  func.b	(ax)+,dx
//--------------------------------------------------------------------
	funcbeadx	\1,18,\2ddai,a0_off(a7)		
	funcbeadx	\1,19,\2ddai,a1_off(a7)		
	funcbeadx	\1,1a,\2dd,(a2)+		
	funcbeadx	\1,1b,\2dd,(a3)+		
	funcbeadx	\1,1c,\2dd,(a4)+		
	funcbeadx	\1,1d,\2dd,(a5)+		
	funcbeadx	\1,1e,\2dd,(a6)+		
	funcbeadx	\1,1f,\2ddai,usp
//--------------------------------------------------------------------
//  func.b	-(ax),dx
//--------------------------------------------------------------------
	funcbeadx	\1,20,\2ddad,a0_off(a7)		
	funcbeadx	\1,21,\2ddad,a1_off(a7)		
	funcbeadx	\1,22,\2dd,-(a2)		
	funcbeadx	\1,23,\2dd,-(a3)		
	funcbeadx	\1,24,\2dd,-(a4)		
	funcbeadx	\1,25,\2dd,-(a5)		
	funcbeadx	\1,26,\2dd,-(a6)		
	funcbeadx	\1,27,\2ddad,usp
//--------------------------------------------------------------------
//  func.b	d16(ax),dx
//--------------------------------------------------------------------
	funcbeadx	\1,28,\2d16ad,a0_off(a7)		
	funcbeadx	\1,29,\2d16ad,a1_off(a7)		
	funcbeadx	\1,2a,\2d16ad,a2		
	funcbeadx	\1,2b,\2d16ad,a3		
	funcbeadx	\1,2c,\2d16ad,a4		
	funcbeadx	\1,2d,\2d16ad,a5		
	funcbeadx	\1,2e,\2d16ad,a6		
	funcbeadx	\1,2f,\2d16ad,usp
//--------------------------------------------------------------------
//  func.b	d8(ax,dy),dx
//--------------------------------------------------------------------
	funcbeadx	\1,30,\2d8ad,a0_off(a7)		
	funcbeadx	\1,31,\2d8ad,a1_off(a7)		
	funcbeadx	\1,32,\2d8ad,a2		
	funcbeadx	\1,33,\2d8ad,a3		
	funcbeadx	\1,34,\2d8ad,a4		
	funcbeadx	\1,35,\2d8ad,a5		
	funcbeadx	\1,36,\2d8ad,a6		
	funcbeadx	\1,37,\2d8ad,usp
//--------------------------------------------------------------------
// func.b xxx.w,dx
//--------------------------------------------------------------------
	funcbeadx	\1,38,\2xwd,(a0)+
//--------------------------------------------------------------------
// func.b xxx.w,dx
//--------------------------------------------------------------------
	funcbeadx	\1,39,\2xld,(a0)+
//--------------------------------------------------------------------
// func.b d16(pc),dx
//--------------------------------------------------------------------
	funcbeadx	\1,3a,\2d16pcd,(a0)+
//--------------------------------------------------------------------
// func.b d8(pc,dy),dx
//--------------------------------------------------------------------
	funcbeadx	\1,3b,\2d8pcd,(a0)+	(a0 wird nicht verwendet)
//--------------------------------------------------------------------
// func.b	#im,dx
//--------------------------------------------------------------------
	funcbeadx	\1,3c,\2bir_macro,(a0)+
//--------------------------------------------------------------------
//  func.b	dy,ea
//--------------------------------------------------------------------
///--------------------------------------------------------------------
//  func.b	dx,dd -> addx subx etc. src und dest vertauscht!
//--------------------------------------------------------------------
	funcbdxea	\1,00,\2dx,d0_off+3(a7)		
	funcbdxea	\1,01,\2dx,d1_off+3(a7)		
	funcbdxea	\1,02,\2dx,d2		
	funcbdxea	\1,03,\2dx,d3		
	funcbdxea	\1,04,\2dx,d4		
	funcbdxea	\1,05,\2dx,d5		
	funcbdxea	\1,06,\2dx,d6		
	funcbdxea	\1,07,\2dx,d7
//--------------------------------------------------------------------
//  func.b	-(ax),-(ay) addx subx etc. src und dest vertauscht!
//--------------------------------------------------------------------
	funcaxay	\1,08,\2dax,a0_off(a7),b		
	funcaxay	\1,09,\2dax,a1_off(a7).b		
	funcaxay	\1,0a,\2dax,a2,b		
	funcaxay	\1,0b,\2dax,a3,b		
	funcaxay	\1,0c,\2dax,a4,b		
	funcaxay	\1,0d,\2dax,a5,b		
	funcaxay	\1,0e,\2dax,a6,b		
	funcaxay	\1,0f,\2dax,usp,b
//--------------------------------------------------------------------
//  func.b	dy,(ax)
//--------------------------------------------------------------------
	funcbdxea	\1,10,\2eda,a0_off(a7)		
	funcbdxea	\1,11,\2eda,a1_off(a7)		
	funcbdxea	\1,12,\2dd,(a2)		
	funcbdxea	\1,13,\2dd,(a3)		
	funcbdxea	\1,14,\2dd,(a4)		
	funcbdxea	\1,15,\2dd,(a5)		
	funcbdxea	\1,16,\2dd,(a6)		
	funcbdxea	\1,17,\2eda,usp
//--------------------------------------------------------------------
//  func.b	dy,(ax)+
//--------------------------------------------------------------------
	funcbdxea	\1,18,\2edai,a0_off(a7)		
	funcbdxea	\1,19,\2edai,a1_off(a7)		
	funcbdxea	\1,1a,\2edaid,(a2)		
	funcbdxea	\1,1b,\2edaid,(a3)	
	funcbdxea	\1,1c,\2edaid,(a4)	
	funcbdxea	\1,1d,\2edaid,(a5)	
	funcbdxea	\1,1e,\2edaid,(a6)		
	funcbdxea	\1,1f,\2edai,usp
//--------------------------------------------------------------------
//  func.b	dy,-(ax)
//--------------------------------------------------------------------
	funcbdxea	\1,20,\2edad,a0_off(a7)		
	funcbdxea	\1,21,\2edad,a1_off(a7)		
	funcbdxea	\1,22,\2edadd,(a2)		
	funcbdxea	\1,23,\2edadd,(a3)		
	funcbdxea	\1,24,\2edadd,(a4)		
	funcbdxea	\1,25,\2edadd,(a5)		
	funcbdxea	\1,26,\2edadd,(a6)		
	funcbdxea	\1,27,\2edad,usp
//--------------------------------------------------------------------
//  func.b	dy,d16(ax)
//--------------------------------------------------------------------
	funcbdxea	\1,28,\2e16ad,a0_off(a7)		
	funcbdxea	\1,29,\2e16ad,a1_off(a7)		
	funcbdxea	\1,2a,\2e16ad,a2		
	funcbdxea	\1,2b,\2e16ad,a3		
	funcbdxea	\1,2c,\2e16ad,a4		
	funcbdxea	\1,2d,\2e16ad,a5		
	funcbdxea	\1,2e,\2e16ad,a6		
	funcbdxea	\1,2f,\2e16ad,usp
//--------------------------------------------------------------------
//  func.b	dy,d8(ax,dy)
//--------------------------------------------------------------------
	funcbdxea	\1,30,\2e8ad,a0_off(a7)		
	funcbdxea	\1,31,\2e8ad,a1_off(a7)		
	funcbdxea	\1,32,\2e8ad,a2		
	funcbdxea	\1,33,\2e8ad,a3		
	funcbdxea	\1,34,\2e8ad,a4		
	funcbdxea	\1,35,\2e8ad,a5		
	funcbdxea	\1,36,\2e8ad,a6		
	funcbdxea	\1,37,\2e8ad,usp
//--------------------------------------------------------------------
// func.w dy,xxx.w
//--------------------------------------------------------------------
	funcwdxea	\1,38,\2xwe,(a0)+
//--------------------------------------------------------------------
// func.w dy,xxx.w
//--------------------------------------------------------------------
	funcwdxea	\1,39,\2xld,(a0)+
/*****************************************************************************************/
// word
/*****************************************************************************************/
//  func.w	ds,dx
//--------------------------------------------------------------------
	funcweadx	\1,40,\2dd,d0_off+2(a7)		
	funcweadx	\1,41,\2dd,d1_off+2(a7)		
	funcweadx	\1,42,\2dd,d2		
	funcweadx	\1,43,\2dd,d3		
	funcweadx	\1,44,\2dd,d4		
	funcweadx	\1,45,\2dd,d5		
	funcweadx	\1,46,\2dd,d6		
	funcweadx	\1,47,\2dd,d7
//--------------------------------------------------------------------
//  func.w	ax,dx
//--------------------------------------------------------------------
	funcweadx	\1,48,\2dd,a0_off+2(a7)		
	funcweadx	\1,49,\2dd,a1_off+2(a7)		
	funcweadx	\1,4a,\2dd,a2		
	funcweadx	\1,4b,\2dd,a3		
	funcweadx	\1,4c,\2dd,a4		
	funcweadx	\1,4d,\2dd,a5		
	funcweadx	\1,4e,\2dd,a6		
	funcweadx	\1,4f,\2ddd,usp
//--------------------------------------------------------------------
//  func.w	(ax),dx
//--------------------------------------------------------------------
	funcweadx	\1,50,\2dda,a0_off(a7)		
	funcweadx	\1,51,\2dda,a1_off(a7)		
	funcweadx	\1,52,\2dd,(a2)		
	funcweadx	\1,53,\2dd,(a3)		
	funcweadx	\1,54,\2dd,(a4)		
	funcweadx	\1,55,\2dd,(a5)		
	funcweadx	\1,56,\2dd,(a6)		
	funcweadx	\1,57,\2dda,usp
//--------------------------------------------------------------------
//  func.w	(ax)+,dx
//--------------------------------------------------------------------
	funcweadx	\1,58,\2ddai,a0_off(a7)		
	funcweadx	\1,59,\2ddai,a1_off(a7)		
	funcweadx	\1,5a,\2dd,(a2)+		
	funcweadx	\1,5b,\2dd,(a3)+		
	funcweadx	\1,5c,\2dd,(a4)+		
	funcweadx	\1,5d,\2dd,(a5)+		
	funcweadx	\1,5e,\2dd,(a6)+		
	funcweadx	\1,5f,\2ddai,usp
//--------------------------------------------------------------------
//  func.w	-(ax),dx
//--------------------------------------------------------------------
	funcweadx	\1,60,\2ddad,a0_off(a7)		
	funcweadx	\1,61,\2ddad,a1_off(a7)		
	funcweadx	\1,62,\2dd,-(a2)		
	funcweadx	\1,63,\2dd,-(a3)		
	funcweadx	\1,64,\2dd,-(a4)		
	funcweadx	\1,65,\2dd,-(a5)		
	funcweadx	\1,66,\2dd,-(a6)		
	funcweadx	\1,67,\2ddad,usp
//--------------------------------------------------------------------
//  func.w	d16(ax),dx
//--------------------------------------------------------------------
	funcweadx	\1,68,\2d16ad,a0_off(a7)		
	funcweadx	\1,69,\2d16ad,a1_off(a7)		
	funcweadx	\1,6a,\2d16ad,a2		
	funcweadx	\1,6b,\2d16ad,a3		
	funcweadx	\1,6c,\2d16ad,a4		
	funcweadx	\1,6d,\2d16ad,a5		
	funcweadx	\1,6e,\2d16ad,a6		
	funcweadx	\1,6f,\2d16ad,usp
//--------------------------------------------------------------------
//  func.w	d8(ax,dy),dx
//--------------------------------------------------------------------
	funcweadx	\1,70,\2d8ad,a0_off(a7)		
	funcweadx	\1,71,\2d8ad,a1_off(a7)		
	funcweadx	\1,72,\2d8ad,a2		
	funcweadx	\1,73,\2d8ad,a3		
	funcweadx	\1,74,\2d8ad,a4		
	funcweadx	\1,75,\2d8ad,a5		
	funcweadx	\1,76,\2d8ad,a6		
	funcweadx	\1,77,\2d8ad,usp
//--------------------------------------------------------------------
// func.w xxx.w,dx
//--------------------------------------------------------------------
	funcweadx	\1,78,\2xwd,(a0)+
//--------------------------------------------------------------------
// func.w xxx.w,dx
//--------------------------------------------------------------------
	funcweadx	\1,79,\2xld,(a0)+
//--------------------------------------------------------------------
// func.w d16(pc),dx
//--------------------------------------------------------------------
	funcweadx	\1,7a,\2d16pcd,(a0)+
//--------------------------------------------------------------------
// func.w d8(pc,dy),dx
//--------------------------------------------------------------------
	funcweadx	\1,7b,\2d8pcd,(a0)+	(a0 wird nicht verwendet)
//--------------------------------------------------------------------
// func.w #im,dx
//--------------------------------------------------------------------
	funcweadx	\1,7c,\2dd,(a0)+
//--------------------------------------------------------------------
//  func.w	dy,ea
//--------------------------------------------------------------------
///--------------------------------------------------------------------
//  func.w	dx,dd -> addx subx etc.
//--------------------------------------------------------------------
.ifnc	\2,and			//platz für exg
	funcwdxea	\1,40,\2dx,d0_off+2(a7)		
	funcwdxea	\1,41,\2dx,d1_off+2(a7)		
	funcwdxea	\1,42,\2dx,d2		
	funcwdxea	\1,43,\2dx,d3		
	funcwdxea	\1,44,\2dx,d4		
	funcwdxea	\1,45,\2dx,d5		
	funcwdxea	\1,46,\2dx,d6		
	funcwdxea	\1,47,\2dx,d7
//--------------------------------------------------------------------
//  func.w	-(ax),-(ay) -> addx,subx
//--------------------------------------------------------------------
	funcaxay	\1,48,\2dax,a0_off(a7),w		
	funcaxay	\1,49,\2dax,a1_off(a7).w		
	funcaxay	\1,4a,\2dax,a2,w		
	funcaxay	\1,4b,\2dax,a3,w		
	funcaxay	\1,4c,\2dax,a4,w		
	funcaxay	\1,4d,\2dax,a5,w		
	funcaxay	\1,4e,\2dax,a6,w		
	funcaxay	\1,4f,\2dax,usp,w
.endif
//--------------------------------------------------------------------
//  func.w	dy,(ax)
//--------------------------------------------------------------------
	funcwdxea	\1,50,\2eda,a0_off(a7)		
	funcwdxea	\1,51,\2eda,a1_off(a7)		
	funcwdxea	\1,52,\2dd,(a2)		
	funcwdxea	\1,53,\2dd,(a3)		
	funcwdxea	\1,54,\2dd,(a4)		
	funcwdxea	\1,55,\2dd,(a5)		
	funcwdxea	\1,56,\2dd,(a6)		
	funcwdxea	\1,57,\2eda,usp
//--------------------------------------------------------------------
//  func.w	dy,(ax)+
//--------------------------------------------------------------------
	funcwdxea	\1,58,\2edai,a0_off(a7)		
	funcwdxea	\1,59,\2edai,a1_off(a7)		
	funcwdxea	\1,5a,\2edaid,(a2)		
	funcwdxea	\1,5b,\2edaid,(a3)		
	funcwdxea	\1,5c,\2edaid,(a4)		
	funcwdxea	\1,5d,\2edaid,(a5)		
	funcwdxea	\1,5e,\2edaid,(a6)		
	funcwdxea	\1,5f,\2edai,usp
//--------------------------------------------------------------------
//  func.w	dy,-(ax)
//--------------------------------------------------------------------
	funcwdxea	\1,60,\2edad,a0_off(a7)		
	funcwdxea	\1,61,\2edad,a1_off(a7)		
	funcwdxea	\1,62,\2edadd,(a2)		
	funcwdxea	\1,63,\2edadd,(a3)		
	funcwdxea	\1,64,\2edadd,(a4)		
	funcwdxea	\1,65,\2edadd,(a5)		
	funcwdxea	\1,66,\2edadd,(a6)		
	funcwdxea	\1,67,\2edad,usp
//--------------------------------------------------------------------
//  func.w	dy,d16(ax)
//--------------------------------------------------------------------
	funcwdxea	\1,68,\2e16ad,a0_off(a7)		
	funcwdxea	\1,69,\2e16ad,a1_off(a7)		
	funcwdxea	\1,6a,\2e16ad,a2		
	funcwdxea	\1,6b,\2e16ad,a3		
	funcwdxea	\1,6c,\2e16ad,a4		
	funcwdxea	\1,6d,\2e16ad,a5		
	funcwdxea	\1,6e,\2e16ad,a6		
	funcwdxea	\1,6f,\2e16ad,usp
//--------------------------------------------------------------------
//  func.w	dy,d8(ax,dy)
//--------------------------------------------------------------------
	funcwdxea	\1,70,\2e8ad,a0_off(a7)		
	funcwdxea	\1,71,\2e8ad,a1_off(a7)		
	funcwdxea	\1,72,\2e8ad,a2		
	funcwdxea	\1,73,\2e8ad,a3		
	funcwdxea	\1,74,\2e8ad,a4		
	funcwdxea	\1,75,\2e8ad,a5		
	funcwdxea	\1,76,\2e8ad,a6		
	funcwdxea	\1,77,\2e8ad,usp
//--------------------------------------------------------------------
// func.w dy,xxx.w
//--------------------------------------------------------------------
	funcwdxea	\1,78,\2xwe,(a0)+
//--------------------------------------------------------------------
// func.w dy,xxx.w
//--------------------------------------------------------------------
	funcwdxea	\1,79,\2xld,(a0)+
/*****************************************************************************************/
// long
/*****************************************************************************************/
//--------------------------------------------------------------------
//  func.l	-(ax),-(ay)
//--------------------------------------------------------------------
	funcaxay	\1,c8,\2dax,a0_off(a7),l		
	funcaxay	\1,c9,\2dax,a1_off(a7).l		
	funcaxay	\1,ca,\2dax,a2,l		
	funcaxay	\1,cb,\2dax,a3,l		
	funcaxay	\1,cc,\2dax,a4,l		
	funcaxay	\1,cd,\2dax,a5,l		
	funcaxay	\1,ce,\2dax,a6,l		
	funcaxay	\1,cf,\2dax,usp,l
//--------------------------------------------------------------------
//  func.l	d8(ax,dy),dx
//--------------------------------------------------------------------
	funcleadx	\1,b0,\2d8ad,a0_off(a7)		
	funcleadx	\1,b1,\2d8ad,a1_off(a7)		
	funcleadx	\1,b2,\2d8ad,a2		
	funcleadx	\1,b3,\2d8ad,a3		
	funcleadx	\1,b4,\2d8ad,a4		
	funcleadx	\1,b5,\2d8ad,a5		
	funcleadx	\1,b6,\2d8ad,a6		
	funcleadx	\1,b7,\2d8ad,usp
//--------------------------------------------------------------------
// func.l d8(pc,dy),dx
//--------------------------------------------------------------------
	funcleadx	\1,bb,\2d8pcd,(a0)+	(a0 wird nicht verwendet)
//--------------------------------------------------------------------
//  func.l	dy,d8(ax,dy)
//--------------------------------------------------------------------
	funcldxea	\1,b0,\2e8ad,a0_off(a7)		
	funcldxea	\1,b1,\2e8ad,a1_off(a7)		
	funcldxea	\1,b2,\2e8ad,a2		
	funcldxea	\1,b3,\2e8ad,a3		
	funcldxea	\1,b4,\2e8ad,a4		
	funcldxea	\1,b5,\2e8ad,a5		
	funcldxea	\1,b6,\2e8ad,a6		
	funcldxea	\1,b7,\2e8ad,usp
/******************************************************/
// adress register
/******************************************************/
//--------------------------------------------------------------------
// func.w ea,ax
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// func.w	dx,ax
//--------------------------------------------------------------------
	funcweaax	\1,c0,\2aw,d0		
	funcweaax	\1,c1,\2aw,d1		
	funcweaax	\1,c2,\2aw,d2		
	funcweaax	\1,c3,\2aw,d3		
	funcweaax	\1,c4,\2aw,d4		
	funcweaax	\1,c5,\2aw,d5		
	funcweaax	\1,c6,\2aw,d6		
	funcweaax	\1,c7,\2aw,d7		
//--------------------------------------------------------------------
// func.w	ay,ax
//--------------------------------------------------------------------
	funcweaax	\1,c8,\2aw,a0		
	funcweaax	\1,c9,\2aw,a1		
	funcweaax	\1,ca,\2aw,a2		
	funcweaax	\1,cb,\2aw,a3		
	funcweaax	\1,cc,\2aw,a4		
	funcweaax	\1,cd,\2aw,a5		
	funcweaax	\1,ce,\2aw,a6		
	funcweaax	\1,cf,\2awu,a7	
//--------------------------------------------------------------------
// func.w	(ay),ax
//--------------------------------------------------------------------
	funcweaax	\1,d0,\2aw,(a0)		
	funcweaax	\1,d1,\2aw,(a1)	
	funcweaax	\1,d2,\2aw,(a2)		
	funcweaax	\1,d3,\2aw,(a3)		
	funcweaax	\1,d4,\2aw,(a4)		
	funcweaax	\1,d5,\2aw,(a5)		
	funcweaax	\1,d6,\2aw,(a6)		
	funcweaax	\1,d7,\2awu,(a7)	
//--------------------------------------------------------------------
// func.w	(ay)+,ax
//--------------------------------------------------------------------
	funcweaax	\1,d8,\2aw,(a0)+		
	funcweaax	\1,d9,\2aw,(a1)+
	funcweaax	\1,da,\2aw,(a2)+	
	funcweaax	\1,db,\2aw,(a3)+		
	funcweaax	\1,dc,\2aw,(a4)+	
	funcweaax	\1,dd,\2aw,(a5)+	
	funcweaax	\1,de,\2aw,(a6)+	
	funcweaax	\1,df,\2awu,(a7)+	
//--------------------------------------------------------------------
// func.w	-(ay),ax
//--------------------------------------------------------------------
	funcweaax	\1,e0,\2aw,-(a0)		
	funcweaax	\1,e1,\2aw,-(a1)
	funcweaax	\1,e2,\2aw,-(a2)	
	funcweaax	\1,e3,\2aw,-(a3)		
	funcweaax	\1,e4,\2aw,-(a4)	
	funcweaax	\1,e5,\2aw,-(a5)	
	funcweaax	\1,e6,\2aw,-(a6)	
	funcweaax	\1,e7,\2awu,-(a7)	
//--------------------------------------------------------------------
// func.w	d16(ay),ax
//--------------------------------------------------------------------
	funcweaaxn	\1,e8,\2awd16a,a0_off(a7)		
	funcweaaxn	\1,e9,\2awd16a,a1_off(a7)
	funcweaaxn	\1,ea,\2awd16a,a2	
	funcweaaxn	\1,eb,\2awd16a,a3		
	funcweaaxn	\1,ec,\2awd16a,a4	
	funcweaaxn	\1,ed,\2awd16a,a5	
	funcweaaxn	\1,ee,\2awd16a,a6	
	funcweaaxn	\1,ef,\2awd16a,usp	
//--------------------------------------------------------------------
// func.w	d8(ay,dy),ax
//--------------------------------------------------------------------
	funcweaaxn	\1,f0,\2awd8a,a0_off(a7)		
	funcweaaxn	\1,f1,\2awd8a,a1_off(a7)
	funcweaaxn	\1,f2,\2awd8a,a2	
	funcweaaxn	\1,f3,\2awd8a,a3		
	funcweaaxn	\1,f4,\2awd8a,a4	
	funcweaaxn	\1,f5,\2awd8a,a5	
	funcweaaxn	\1,f6,\2awd8a,a6	
	funcweaaxn	\1,f7,\2awd8a,usp	
//--------------------------------------------------------------------
// func.w xxx.w,ax
//--------------------------------------------------------------------
	funcweaaxn	\1,f8,\2awxwax,(a0)+
//--------------------------------------------------------------------
// func.w xxxlw,ax
//--------------------------------------------------------------------
	funcweaaxn	\1,f9,\2awxlax,(a0)+
//--------------------------------------------------------------------
// func.w d16(pc),ax
//--------------------------------------------------------------------
	funcweaaxn	\1,fa,\2awd16pcax,(a0)+
//--------------------------------------------------------------------
// func.w d8(pc,dy),ax
//--------------------------------------------------------------------
	funcweaaxn	\1,fb,\2awd8pcax,(a0)+	//(a0 wird nicht verwendet)
//--------------------------------------------------------------------
// func.w #im,ax
//--------------------------------------------------------------------
	funcweaaxn	\1,fc,\2awim,(a0)+	
//--------------------------------------------------------------------
// ende
	.endm;	
//--------------------------------------------------------------------
// byt
funcbeadx:.macro					// function byt: im,dx 
ii_0x\10\2:			 
	\3		\4,d0_off+3(a7),b
ii_0x\12\2:				 
	\3		\4,d1_off+3(a7),b
ii_0x\14\2:			 
	\3		\4,d2,b
ii_0x\16\2:			 
	\3		\4,d3,b 
ii_0x\18\2:			 
	\3		\4,d4,b 
ii_0x\1a\2:				 
	\3		\4,d5,b
ii_0x\1c\2:			 
	\3		\4,d6,b
ii_0x\1e\2:			 
	\3		\4,d7,b
	.endm;
funcbdxea:.macro	// ea(\4) function(\3) dx -> ea
ii_0x\11\2:
	\3	d0_off+3(a7),\4,b
ii_0x\13\2:
	\3	d1_off+3(a7),\4,b
ii_0x\15\2:
	\3	d2,\4,b
ii_0x\17\2:
	\3	d3,\4,b
ii_0x\19\2:
	\3	d4,\4,b
ii_0x\1b\2:
	\3	d5,\4,b
ii_0x\1d\2:
	\3	d6,\4,b
ii_0x\1f\2:
	\3	d7,\4,b
	.endm;
//--------------------------------------------------------------------
// word
funcweadx:.macro	// dx function(\3) ea(\4) -> dx 
ii_0x\10\2:
	\3	\4,d0_off+2(a7),w
ii_0x\12\2:
	\3	\4,d1_off+2(a7),w
ii_0x\14\2:
	\3	\4,d2,w
ii_0x\16\2:
	\3	\4,d3,w
ii_0x\18\2:
	\3	\4,d4,w
ii_0x\1a\2:
	\3	\4,d5,w
ii_0x\1c\2:
	\3	\4,d6,w
ii_0x\1e\2:
	\3	\4,d7,w
	.endm;
funcwdxea:.macro	// ea(\4) function(\3) dx -> ea 
ii_0x\11\2:
	\3	d0_off+2(a7),\4,w
ii_0x\13\2:
	\3	d1_off+2(a7),\4,w
ii_0x\15\2:
	\3	d2,\4,w
ii_0x\17\2:
	\3	d3,\4,w
ii_0x\19\2:
	\3	d4,\4,w
ii_0x\1b\2:
	\3	d5,\4,w
ii_0x\1d\2:
	\3	d6,\4,w
ii_0x\1f\2:
	\3	d7,\4,w
	.endm;
//--------------------------------------------------------------------
// long
funcleadx:.macro	// dx function(\3) ea(\4) -> dx 
ii_0x\10\2:
	\3	\4,d0_off(a7),w
ii_0x\12\2:
	\3	\4,d1_off(a7),w
ii_0x\14\2:
	\3	\4,d2,w
ii_0x\16\2:
	\3	\4,d3,w
ii_0x\18\2:
	\3	\4,d4,w
ii_0x\1a\2:
	\3	\4,d5,w
ii_0x\1c\2:
	\3	\4,d6,w
ii_0x\1e\2:
	\3	\4,d7,w
	.endm;
funcldxea:.macro	// ea(\4) function(\3) dx -> ea 
ii_0x\11\2:
	\3	d0_off(a7),\4,w
ii_0x\13\2:
	\3	d1_off(a7),\4,w
ii_0x\15\2:
	\3	d2,\4,w
ii_0x\17\2:
	\3	d3,\4,w
ii_0x\19\2:
	\3	d4,\4,w
ii_0x\1b\2:
	\3	d5,\4,w
ii_0x\1d\2:
	\3	d6,\4,w
ii_0x\1f\2:
	\3	d7,\4,w
	.endm;
//--------------------------------------------------------------
// address
funcweaax:.macro	// ax function(\3) ea(\4)(ext long!) -> ax 
ii_0x\10\2:
	\3	\4,a0
ii_0x\12\2:
	\3	\4,a1
ii_0x\14\2:
	\3	\4,a2 
ii_0x\16\2:
	\3	\4,a3 
ii_0x\18\2:
	\3	\4,a4 
ii_0x\1a\2:
	\3	\4,a5 
ii_0x\1c\2:
	\3	\4,a6 
ii_0x\1e\2:
	\3a7 \4,a7 		// "a7" beachten wegen usp
	.endm;
funcweaaxn:.macro	// ax function(\3) ea(\4)(ext long!) -> ax 
ii_0x\10\2:
	\3	\4,a0_off(a7) 
ii_0x\12\2:
	\3	\4,a1_off(a7) 
ii_0x\14\2:
	\3	\4,a2 
ii_0x\16\2:
	\3	\4,a3 
ii_0x\18\2:
	\3	\4,a4 
ii_0x\1a\2:
	\3	\4,a5 
ii_0x\1c\2:
	\3	\4,a6 
ii_0x\1e\2:
	\3	\4,usp 
	.endm;
//--------------------------------------------------------------
// byt, word, long
//--------------------------------------------------------------
funcaxay:.macro	// ea(\4) function(\3) dx -> ea,\5 = size
ii_0x\11\2:
	\3	a0_off(a7),\4,\5
ii_0x\13\2:
	\3	a1_off(a7),\4,\5
ii_0x\15\2:
	\3	a2,\4,\5
ii_0x\17\2:
	\3	a3,\4,\5
ii_0x\19\2:
	\3	a4,\4,\5
ii_0x\1b\2:
	\3	a5,\4,\5
ii_0x\1d\2:
	\3	a6,\4,\5
ii_0x\1f\2:
	\3	usp,\4,\5
	.endm;
