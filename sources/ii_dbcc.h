//--------------------------------------------------------------------
// dbcc,trapcc
//--------------------------------------------------------------------
.text
ii_lset_dbcc:.macro 
// dbra
	ii_lset_opeau	51,c
	ii_lset_opeau	52,c
	ii_lset_opeau	53,c
	ii_lset_opeau	54,c
	ii_lset_opeau	55,c
	ii_lset_opeau	56,c
	ii_lset_opeau	57,c
	ii_lset_opeau	58,c
	ii_lset_opeau	59,c
	ii_lset_opeau	5a,c
	ii_lset_opeau	5b,c
	ii_lset_opeau	5c,c
	ii_lset_opeau	5d,c
	ii_lset_opeau	5e,c
	ii_lset_opeau	5f,c
.endm

ii_dbcc_func:.macro
ii_0x51c8:
		dbra_macro	d0_off+2(a7)
ii_0x51c9:
		dbra_macro	d1_off+2(a7)
ii_0x51ca:
		dbra_macro	d2
ii_0x51cb:
		dbra_macro	d3
ii_0x51cc:
		dbra_macro	d4
ii_0x51cd:
		dbra_macro	d5
ii_0x51ce:
		dbra_macro	d6
ii_0x51cf:
		dbra_macro	d7
//---------------------------------------------------------------------------------------------
// dbcc dx
//---------------------------------------------------------------------------------------------
		ii_dbcc	2,hi
		ii_dbcc 3,ls
		ii_dbcc	4,cc
		ii_dbcc 5,cs
		ii_dbcc	6,ne
		ii_dbcc	7,eq
		ii_dbcc	8,vc
		ii_dbcc 9,vs
		ii_dbcc a,pl
		ii_dbcc b,mi
		ii_dbcc c,ge
		ii_dbcc	d,lt
		ii_dbcc	e,gt
		ii_dbcc	f,le
.endm		
//---------------------------------------------------------------------------------------------
// dbra dx
//---------------------------------------------------------------------------------------------
dbra_macro:.macro
#ifdef	halten_dbcc
	halt
#endif
		mvz.w	\1,d1				// dx holen
		subq.l	#1,d1				// dx-1
		bcc		dbra\@				// bra if plus?
		addq.l	#2,a0				// offset überspringen		
		move.w	d1,\1				// dx sichern
 		ii_end
dbra\@:
		move.w	(a0),a1				// offset (wird auf long erweitert)
		add.l	a1,a0				// dazuadieren
		move.w	d1,\1				// dx sichern
 		ii_end
.endm
//---------------------------------------------------------------------------------------------
// dbcc dx
//---------------------------------------------------------------------------------------------
dbcc_macro:.macro
#ifdef	halten_dbcc
	halt
#endif
		b\2		dbncc\@		
		mvz.w	\1,d1				// dx holen
		subq.l	#1,d1				// dx-1
		bcc		dbcc\@				// bra if plus?
dbncc\@:
		addq.l	#2,a0				// offset überspringen		
		move.w	d1,\1				// dx sichern
 		ii_end
dbcc\@:
		move.w	(a0),a1				// offset (wird auf long erweitert)
		add.l	a1,a0				// dazuadieren
		move.w	d1,\1				// dx sichern
 		ii_end
.endm
//db
ii_dbcc:.macro
ii_0x5\1c8:
		dbcc_macro	d0_off+2(a7),\2
ii_0x5\1c9:
		dbcc_macro	d1_off+2(a7),\2
ii_0x5\1ca:
		dbcc_macro	d2,\2
ii_0x5\1cb:
		dbcc_macro	d3,\2
ii_0x5\1cc:
		dbcc_macro	d4,\2
ii_0x5\1cd:
		dbcc_macro	d5,\2
ii_0x5\1ce:
		dbcc_macro	d6,\2
ii_0x5\1cf:
		dbcc_macro	d7,\2
.endm