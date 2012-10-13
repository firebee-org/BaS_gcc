/*************************************************************************************************/
// extension word format: a0 zeigt auf code, in a1 ist ay, d0/d1 wird zerstört
//------------------------------------------------------------------------------

#include	"ii_macro.h"

.global	ewf
//-----------------------------------------------------------
.text
ewferr:
	nop
	halt
	nop
//-----------------------------------------------------------
ewf:
		mvz.b	(a0)+,d1							// 1. byt ewf
		mvs.w	ewf_table-.-2(pc,d1*2),d1
		jmp		ewf_table-.-2(pc,d1)
ewf_table:				
	.short		ewf_00-ewf_table,ewf_01-ewf_table,ewf_02-ewf_table,ewf_03-ewf_table
	.short		ewf_04-ewf_table,ewf_05-ewf_table,ewf_06-ewf_table,ewf_07-ewf_table
	.short		ewferr-ewf_table,ewf_09-ewf_table,ewferr-ewf_table,ewf_0b-ewf_table
	.short		ewferr-ewf_table,ewf_0d-ewf_table,ewferr-ewf_table,ewf_0f-ewf_table
	.short		ewf_10-ewf_table,ewf_11-ewf_table,ewf_12-ewf_table,ewf_13-ewf_table
	.short		ewf_14-ewf_table,ewf_15-ewf_table,ewf_16-ewf_table,ewf_17-ewf_table
	.short		ewferr-ewf_table,ewf_19-ewf_table,ewferr-ewf_table,ewf_1b-ewf_table
	.short		ewferr-ewf_table,ewf_1d-ewf_table,ewferr-ewf_table,ewf_1f-ewf_table
	.short		ewf_20-ewf_table,ewf_21-ewf_table,ewf_22-ewf_table,ewf_23-ewf_table
	.short		ewf_24-ewf_table,ewf_25-ewf_table,ewf_26-ewf_table,ewf_27-ewf_table
	.short		ewferr-ewf_table,ewf_29-ewf_table,ewferr-ewf_table,ewf_2b-ewf_table
	.short		ewferr-ewf_table,ewf_2d-ewf_table,ewferr-ewf_table,ewf_2f-ewf_table
	.short		ewf_30-ewf_table,ewf_31-ewf_table,ewf_32-ewf_table,ewf_33-ewf_table
	.short		ewf_34-ewf_table,ewf_35-ewf_table,ewf_36-ewf_table,ewf_37-ewf_table
	.short		ewferr-ewf_table,ewf_39-ewf_table,ewferr-ewf_table,ewf_3b-ewf_table
	.short		ewferr-ewf_table,ewf_3d-ewf_table,ewferr-ewf_table,ewf_3f-ewf_table
	.short		ewf_40-ewf_table,ewf_41-ewf_table,ewf_42-ewf_table,ewf_43-ewf_table
	.short		ewf_44-ewf_table,ewf_45-ewf_table,ewf_46-ewf_table,ewf_47-ewf_table
	.short		ewferr-ewf_table,ewf_49-ewf_table,ewferr-ewf_table,ewf_4b-ewf_table
	.short		ewferr-ewf_table,ewf_4d-ewf_table,ewferr-ewf_table,ewf_4f-ewf_table
	.short		ewf_50-ewf_table,ewf_51-ewf_table,ewf_52-ewf_table,ewf_53-ewf_table
	.short		ewf_54-ewf_table,ewf_55-ewf_table,ewf_56-ewf_table,ewf_57-ewf_table
	.short		ewferr-ewf_table,ewf_59-ewf_table,ewferr-ewf_table,ewf_5b-ewf_table
	.short		ewferr-ewf_table,ewf_5d-ewf_table,ewferr-ewf_table,ewf_5f-ewf_table
	.short		ewf_60-ewf_table,ewf_61-ewf_table,ewf_62-ewf_table,ewf_63-ewf_table
	.short		ewf_64-ewf_table,ewf_65-ewf_table,ewf_66-ewf_table,ewf_67-ewf_table
	.short		ewferr-ewf_table,ewf_69-ewf_table,ewferr-ewf_table,ewf_6b-ewf_table
	.short		ewferr-ewf_table,ewf_6d-ewf_table,ewferr-ewf_table,ewf_6f-ewf_table
	.short		ewf_70-ewf_table,ewf_71-ewf_table,ewf_72-ewf_table,ewf_73-ewf_table
	.short		ewf_74-ewf_table,ewf_75-ewf_table,ewf_76-ewf_table,ewf_77-ewf_table
	.short		ewferr-ewf_table,ewf_79-ewf_table,ewferr-ewf_table,ewf_7b-ewf_table
	.short		ewferr-ewf_table,ewf_7d-ewf_table,ewferr-ewf_table,ewf_7f-ewf_table
	.short		ewf_80-ewf_table,ewf_81-ewf_table,ewf_82-ewf_table,ewf_83-ewf_table
	.short		ewf_84-ewf_table,ewf_85-ewf_table,ewf_86-ewf_table,ewf_87-ewf_table
	.short		ewferr-ewf_table,ewf_89-ewf_table,ewferr-ewf_table,ewf_8b-ewf_table
	.short		ewferr-ewf_table,ewf_8d-ewf_table,ewferr-ewf_table,ewf_8f-ewf_table
	.short		ewf_90-ewf_table,ewf_91-ewf_table,ewf_92-ewf_table,ewf_93-ewf_table
	.short		ewf_94-ewf_table,ewf_95-ewf_table,ewf_96-ewf_table,ewf_97-ewf_table
	.short		ewferr-ewf_table,ewf_99-ewf_table,ewferr-ewf_table,ewf_9b-ewf_table
	.short		ewferr-ewf_table,ewf_9d-ewf_table,ewferr-ewf_table,ewf_9f-ewf_table
	.short		ewf_a0-ewf_table,ewf_a1-ewf_table,ewf_a2-ewf_table,ewf_a3-ewf_table
	.short		ewf_a4-ewf_table,ewf_a5-ewf_table,ewf_a6-ewf_table,ewf_a7-ewf_table
	.short		ewferr-ewf_table,ewf_a9-ewf_table,ewferr-ewf_table,ewf_ab-ewf_table
	.short		ewferr-ewf_table,ewf_ad-ewf_table,ewferr-ewf_table,ewf_af-ewf_table
	.short		ewf_b0-ewf_table,ewf_b1-ewf_table,ewf_b2-ewf_table,ewf_b3-ewf_table
	.short		ewf_b4-ewf_table,ewf_b5-ewf_table,ewf_b6-ewf_table,ewf_b7-ewf_table
	.short		ewferr-ewf_table,ewf_b9-ewf_table,ewferr-ewf_table,ewf_bb-ewf_table
	.short		ewferr-ewf_table,ewf_bd-ewf_table,ewferr-ewf_table,ewf_bf-ewf_table
	.short		ewf_c0-ewf_table,ewf_c1-ewf_table,ewf_c2-ewf_table,ewf_c3-ewf_table
	.short		ewf_c4-ewf_table,ewf_c5-ewf_table,ewf_c6-ewf_table,ewf_c7-ewf_table
	.short		ewferr-ewf_table,ewf_c9-ewf_table,ewferr-ewf_table,ewf_cb-ewf_table
	.short		ewferr-ewf_table,ewf_cd-ewf_table,ewferr-ewf_table,ewf_cf-ewf_table
	.short		ewf_d0-ewf_table,ewf_d1-ewf_table,ewf_d2-ewf_table,ewf_d3-ewf_table
	.short		ewf_d4-ewf_table,ewf_d5-ewf_table,ewf_d6-ewf_table,ewf_d7-ewf_table
	.short		ewferr-ewf_table,ewf_d9-ewf_table,ewferr-ewf_table,ewf_db-ewf_table
	.short		ewferr-ewf_table,ewf_dd-ewf_table,ewferr-ewf_table,ewf_df-ewf_table
	.short		ewf_e0-ewf_table,ewf_e1-ewf_table,ewf_e2-ewf_table,ewf_e3-ewf_table
	.short		ewf_e4-ewf_table,ewf_e5-ewf_table,ewf_e6-ewf_table,ewf_e7-ewf_table
	.short		ewferr-ewf_table,ewf_e9-ewf_table,ewferr-ewf_table,ewf_eb-ewf_table
	.short		ewferr-ewf_table,ewf_ed-ewf_table,ewferr-ewf_table,ewf_ef-ewf_table
	.short		ewf_f0-ewf_table,ewf_f1-ewf_table,ewf_f2-ewf_table,ewf_f3-ewf_table
	.short		ewf_f4-ewf_table,ewf_f5-ewf_table,ewf_f6-ewf_table,ewf_f7-ewf_table
	.short		ewferr-ewf_table,ewf_f9-ewf_table,ewferr-ewf_table,ewf_fb-ewf_table
	.short		ewferr-ewf_table,ewf_fd-ewf_table,ewferr-ewf_table,ewf_ff-ewf_table
//d0.w * 1
ewf_00:
	mvs.b		(a0)+,d1
	mvs.w		d0_off+6(a7),d0
	add.l		d0,a1
	add.l		d1,a1
	rts
ewf_01:
	mvs.w		d0_off+6(a7),d0
	bra			ewf_full
//d0.w * 2
ewf_02:
	mvs.b		(a0)+,d1
	mvs.w		d0_off+6(a7),d0
	lea			0(a1,d0*2),a1
	add.l		d1,a1
	rts
ewf_03:
	mvs.w		d0_off+6(a7),d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//d0.w * 4
ewf_04:
	mvs.b		(a0)+,d1
	mvs.w		d0_off+6(a7),d0
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_05:
	mvs.w		d0_off+6(a7),d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//d0.w * 8
ewf_06:
	mvs.b		(a0)+,d1
	mvs.w		d0_off+6(a7),d0
	lea			0(a1,d0*4),a1
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_07:
	mvs.w		d0_off+6(a7),d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//d0.l * 1
ewf_09:
	move.l		d0_off+4(a7),d0
	bra			ewf_full
//d0.l * 2
ewf_0b:
	move.l		d0_off+4(a7),d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//d0.l * 4
ewf_0d:
	move.l		d0_off+4(a7),d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//d0.l * 8
ewf_0f:
	move.l		d0_off+4(a7),d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//d1.w * 1
ewf_10:
	mvs.b		(a0)+,d1
	mvs.w		d1_off+6(a7),d0
	add.l		d0,a1
	add.l		d1,a1
	rts
ewf_11:
	mvs.w		d1_off+6(a7),d0
	bra			ewf_full
//d1.w * 2
ewf_12:
	mvs.b		(a0)+,d1
	mvs.w		d1_off+6(a7),d0
	lea			0(a1,d0*2),a1
	add.l		d1,a1
	rts
ewf_13:
	mvs.w		d1_off+6(a7),d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//d1.w * 4
ewf_14:
	mvs.b		(a0)+,d1
	mvs.w		d1_off+6(a7),d0
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_15:
	mvs.w		d1_off+6(a7),d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//d1.w * 8
ewf_16:
	mvs.b		(a0)+,d1
	mvs.w		d1_off+6(a7),d0
	lea			0(a1,d0*4),a1
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_17:
	mvs.w		d1_off+6(a7),d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//d1.l * 1
ewf_19:
	move.l		d1_off+4(a7),d0
	bra			ewf_full
//d1.l * 2
ewf_1b:
	move.l		d1_off+4(a7),d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//d1.l * 4
ewf_1d:
	move.l		d1_off+4(a7),d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//d1.l * 8
ewf_1f:
	move.l		d1_off+4(a7),d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//d2.w * 1
ewf_20:
	mvs.b		(a0)+,d1
	mvs.w		d2,d0
	add.l		d0,a1
	add.l		d1,a1
	rts
ewf_21:
	mvs.w		d2,d0
	bra			ewf_full
//d2.w * 2
ewf_22:
	mvs.b		(a0)+,d1
	mvs.w		d2,d0
	lea			0(a1,d0*2),a1
	add.l		d1,a1
	rts
ewf_23:
	mvs.w		d2,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//d2.w * 4
ewf_24:
	mvs.b		(a0)+,d1
	mvs.w		d2,d0
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_25:
	mvs.w		d2,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//d2.w * 8
ewf_26:
	mvs.b		(a0)+,d1
	mvs.w		d2,d0
	lea			0(a1,d0*4),a1
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_27:
	mvs.w		d2,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//d2.l * 1
ewf_29:
	move.l		d2,d0
	bra			ewf_full
//d2.l * 2
ewf_2b:
	move.l		d2,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//d2.l * 4
ewf_2d:
	move.l		d2,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//d2.l * 8
ewf_2f:
	move.l		d2,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//d3.w * 1
ewf_30:
	mvs.b		(a0)+,d1
	mvs.w		d3,d0
	add.l		d0,a1
	add.l		d1,a1
	rts
ewf_31:
	mvs.w		d3,d0
	bra			ewf_full
//d3.w * 2
ewf_32:
	mvs.b		(a0)+,d1
	mvs.w		d3,d0
	lea			0(a1,d0*2),a1
	add.l		d1,a1
	rts
ewf_33:
	mvs.w		d3,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//d3.w * 4
ewf_34:
	mvs.b		(a0)+,d1
	mvs.w		d3,d0
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_35:
	mvs.w		d3,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//d3.w * 8
ewf_36:
	mvs.b		(a0)+,d1
	mvs.w		d3,d0
	lea			0(a1,d0*4),a1
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_37:
	mvs.w		d3,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//d3.l * 1
ewf_39:
	move.l		d3,d0
	bra			ewf_full
//d3.l * 3
ewf_3b:
	move.l		d3,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//d3.l * 4
ewf_3d:
	move.l		d3,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//d3.l * 8
ewf_3f:
	move.l		d3,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//d4.w * 1
ewf_40:
	mvs.b		(a0)+,d1
	mvs.w		d4,d0
	add.l		d0,a1
	add.l		d1,a1
	rts
ewf_41:
	mvs.w		d4,d0
	bra			ewf_full
//d4.w * 2
ewf_42:
	mvs.b		(a0)+,d1
	mvs.w		d4,d0
	lea			0(a1,d0*2),a1
	add.l		d1,a1
	rts
ewf_43:
	mvs.w		d4,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//d4.w * 4
ewf_44:
	mvs.b		(a0)+,d1
	mvs.w		d4,d0
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_45:
	mvs.w		d4,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//d4.w * 8
ewf_46:
	mvs.b		(a0)+,d1
	mvs.w		d4,d0
	lea			0(a1,d0*4),a1
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_47:
	mvs.w		d4,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//d4.l * 1
ewf_49:
	move.l		d4,d0
	bra			ewf_full
//d4.l * 4
ewf_4b:
	move.l		d4,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//d4.l * 4
ewf_4d:
	move.l		d4,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//d4.l * 8
ewf_4f:
	move.l		d4,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//d5.w * 1
ewf_50:
	mvs.b		(a0)+,d1
	mvs.w		d5,d0
	add.l		d0,a1
	add.l		d1,a1
	rts
ewf_51:
	mvs.w		d5,d0
	bra			ewf_full
//d5.w * 2
ewf_52:
	mvs.b		(a0)+,d1
	mvs.w		d5,d0
	lea			0(a1,d0*2),a1
	add.l		d1,a1
	rts
ewf_53:
	mvs.w		d5,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//d5.w * 4
ewf_54:
	mvs.b		(a0)+,d1
	mvs.w		d5,d0
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_55:
	mvs.w		d5,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//d5.w * 8
ewf_56:
	mvs.b		(a0)+,d1
	mvs.w		d5,d0
	lea			0(a1,d0*4),a1
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_57:
	mvs.w		d5,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//d5.l * 1
ewf_59:
	move.l		d5,d0
	bra			ewf_full
//d5.l * 5
ewf_5b:
	move.l		d5,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//d5.l * 4
ewf_5d:
	move.l		d5,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//d5.l * 8
ewf_5f:
	move.l		d5,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//d6.w * 1
ewf_60:
	mvs.b		(a0)+,d1
	mvs.w		d6,d0
	add.l		d0,a1
	add.l		d1,a1
	rts
ewf_61:
	mvs.w		d6,d0
	bra			ewf_full
//d6.w * 2
ewf_62:
	mvs.b		(a0)+,d1
	mvs.w		d6,d0
	lea			0(a1,d0*2),a1
	add.l		d1,a1
	rts
ewf_63:
	mvs.w		d6,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//d6.w * 4
ewf_64:
	mvs.b		(a0)+,d1
	mvs.w		d6,d0
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_65:
	mvs.w		d6,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//d6.w * 8
ewf_66:
	mvs.b		(a0)+,d1
	mvs.w		d6,d0
	lea			0(a1,d0*4),a1
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_67:
	mvs.w		d6,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//d6.l * 1
ewf_69:
	move.l		d6,d0
	bra			ewf_full
//d6.l * 6
ewf_6b:
	move.l		d6,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//d6.l * 4
ewf_6d:
	move.l		d6,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//d6.l * 8
ewf_6f:
	move.l		d6,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//d7.w * 1
ewf_70:
	mvs.b		(a0)+,d1
	mvs.w		d7,d0
	add.l		d0,a1
	add.l		d1,a1
	rts
ewf_71:
	mvs.w		d7,d0
	bra			ewf_full
//d7.w * 2
ewf_72:
	mvs.b		(a0)+,d1
	mvs.w		d7,d0
	lea			0(a1,d0*2),a1
	add.l		d1,a1
	rts
ewf_73:
	mvs.w		d7,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//d7.w * 4
ewf_74:
	mvs.b		(a0)+,d1
	mvs.w		d7,d0
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_75:
	mvs.w		d7,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//d7.w * 8
ewf_76:
	mvs.b		(a0)+,d1
	mvs.w		d7,d0
	lea			0(a1,d0*4),a1
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_77:
	mvs.w		d7,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//d7.l * 1
ewf_79:
	move.l		d7,d0
	bra			ewf_full
//d7.l * 7
ewf_7b:
	move.l		d7,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//d7.l * 4
ewf_7d:
	move.l		d7,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//d7.l * 8
ewf_7f:
	move.l		d7,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//a0.w * 1
ewf_80:
	mvs.b		(a0)+,d1
	mvs.w		a0_off+6(a7),d0
	add.l		d0,a1
	add.l		d1,a1
	rts
ewf_81:
	mvs.w		a0_off+6(a7),d0
	bra			ewf_full
//a0.w * 2
ewf_82:
	mvs.b		(a0)+,d1
	mvs.w		a0_off+6(a7),d0
	lea			0(a1,d0*2),a1
	add.l		d1,a1
	rts
ewf_83:
	mvs.w		a0_off+6(a7),d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//a0.w * 4
ewf_84:
	mvs.b		(a0)+,d1
	mvs.w		a0_off+6(a7),d0
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_85:
	mvs.w		a0_off+6(a7),d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//a0.w * 8
ewf_86:
	mvs.b		(a0)+,d1
	mvs.w		a0_off+6(a7),d0
	lea			0(a1,d0*4),a1
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_87:
	mvs.w		a0_off+6(a7),d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//a0.l * 1
ewf_89:
	move.l		a0_off+4(a7),d0
	bra			ewf_full
//a0.l * 2
ewf_8b:
	move.l		a0_off+4(a7),d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//a0.l * 4
ewf_8d:
	move.l		a0_off+4(a7),d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//a0.l * 8
ewf_8f:
	move.l		a0_off+4(a7),d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//a1.w * 1
ewf_90:
	mvs.b		(a0)+,d1
	mvs.w		a1_off+6(a7),d0
	add.l		d0,a1
	add.l		d1,a1
	rts
ewf_91:
	mvs.w		a1_off+6(a7),d0
	bra			ewf_full
//a1.w * 2
ewf_92:
	mvs.b		(a0)+,d1
	mvs.w		a1_off+6(a7),d0
	lea			0(a1,d0*2),a1
	add.l		d1,a1
	rts
ewf_93:
	mvs.w		a1_off+6(a7),d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//a1.w * 4
ewf_94:
	mvs.b		(a0)+,d1
	mvs.w		a1_off+6(a7),d0
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_95:
	mvs.w		a1_off+6(a7),d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//a1.w * 8
ewf_96:
	mvs.b		(a0)+,d1
	mvs.w		a1_off+6(a7),d0
	lea			0(a1,d0*4),a1
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_97:
	mvs.w		a1_off+6(a7),d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//a1.l * 1
ewf_99:
	move.l		a1_off+4(a7),d0
	bra			ewf_full
//a1.l * 2
ewf_9b:
	move.l		a1_off+4(a7),d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//a1.l * 4
ewf_9d:
	move.l		a1_off+4(a7),d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//a1.l * 8
ewf_9f:
	move.l		a1_off+4(a7),d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//a2.w * 1
ewf_a0:
	mvs.b		(a0)+,d1
	mvs.w		a2,d0
	add.l		d0,a1
	add.l		d1,a1
	rts
ewf_a1:
	mvs.w		a2,d0
	bra			ewf_full
//a2.w * 2
ewf_a2:
	mvs.b		(a0)+,d1
	mvs.w		a2,d0
	lea			0(a1,d0*2),a1
	add.l		d1,a1
	rts
ewf_a3:
	mvs.w		a2,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//a2.w * 4
ewf_a4:
	mvs.b		(a0)+,d1
	mvs.w		a2,d0
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_a5:
	mvs.w		a2,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//a2.w * 8
ewf_a6:
	mvs.b		(a0)+,d1
	mvs.w		a2,d0
	lea			0(a1,d0*4),a1
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_a7:
	mvs.w		a2,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//a2.l * 1
ewf_a9:
	move.l		a2,d0
	bra			ewf_full
//a2.l * 2
ewf_ab:
	move.l		a2,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//a2.l * 4
ewf_ad:
	move.l		a2,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//a2.l * 8
ewf_af:
	move.l		a2,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//a3.w * 1
ewf_b0:
	mvs.b		(a0)+,d1
	mvs.w		a3,d0
	add.l		d0,a1
	add.l		d1,a1
	rts
ewf_b1:
	mvs.w		a3,d0
	bra			ewf_full
//a3.w * 2
ewf_b2:
	mvs.b		(a0)+,d1
	mvs.w		a3,d0
	lea			0(a1,d0*2),a1
	add.l		d1,a1
	rts
ewf_b3:
	mvs.w		a3,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//a3.w * 4
ewf_b4:
	mvs.b		(a0)+,d1
	mvs.w		a3,d0
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_b5:
	mvs.w		a3,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//a3.w * 8
ewf_b6:
	mvs.b		(a0)+,d1
	mvs.w		a3,d0
	lea			0(a1,d0*4),a1
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_b7:
	mvs.w		a3,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//a3.l * 1
ewf_b9:
	move.l		a3,d0
	bra			ewf_full
//a3.l * 3
ewf_bb:
	move.l		a3,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//a3.l * 4
ewf_bd:
	move.l		a3,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//a3.l * 8
ewf_bf:
	move.l		a3,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//a4.w * 1
ewf_c0:
	mvs.b		(a0)+,d1
	mvs.w		a4,d0
	add.l		d0,a1
	add.l		d1,a1
	rts
ewf_c1:
	mvs.w		a4,d0
	bra			ewf_full
//a4.w * 2
ewf_c2:
	mvs.b		(a0)+,d1
	mvs.w		a4,d0
	lea			0(a1,d0*2),a1
	add.l		d1,a1
	rts
ewf_c3:
	mvs.w		a4,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//a4.w * 4
ewf_c4:
	mvs.b		(a0)+,d1
	mvs.w		a4,d0
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_c5:
	mvs.w		a4,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//a4.w * 8
ewf_c6:
	mvs.b		(a0)+,d1
	mvs.w		a4,d0
	lea			0(a1,d0*4),a1
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_c7:
	mvs.w		a4,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//a4.l * 1
ewf_c9:
	move.l		a4,d0
	bra			ewf_full
//a4.l * 4
ewf_cb:
	move.l		a4,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//a4.l * 4
ewf_cd:
	move.l		a4,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//a4.l * 8
ewf_cf:
	move.l		a4,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//a5.w * 1
ewf_d0:
	mvs.b		(a0)+,d1
	mvs.w		a5,d0
	add.l		d0,a1
	add.l		d1,a1
	rts
ewf_d1:
	mvs.w		a5,d0
	bra			ewf_full
//a5.w * 2
ewf_d2:
	mvs.b		(a0)+,d1
	mvs.w		a5,d0
	lea			0(a1,d0*2),a1
	add.l		d1,a1
	rts
ewf_d3:
	mvs.w		a5,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//a5.w * 4
ewf_d4:
	mvs.b		(a0)+,d1
	mvs.w		a5,d0
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_d5:
	mvs.w		a5,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//a5.w * 8
ewf_d6:
	mvs.b		(a0)+,d1
	mvs.w		a5,d0
	lea			0(a1,d0*4),a1
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_d7:
	mvs.w		a5,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//a5.l * 1
ewf_d9:
	move.l		a5,d0
	bra			ewf_full
//a5.l * 5
ewf_db:
	move.l		a5,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//a5.l * 4
ewf_dd:
	move.l		a5,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//a5.l * 8
ewf_df:
	move.l		a5,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//a6.w * 1
ewf_e0:
	mvs.b		(a0)+,d1
	mvs.w		a6,d0
	add.l		d0,a1
	add.l		d1,a1
	rts
ewf_e1:
	mvs.w		a6,d0
	bra			ewf_full
//a6.w * 2
ewf_e2:
	mvs.b		(a0)+,d1
	mvs.w		a6,d0
	lea			0(a1,d0*2),a1
	add.l		d1,a1
	rts
ewf_e3:
	mvs.w		a6,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//a6.w * 4
ewf_e4:
	mvs.b		(a0)+,d1
	mvs.w		a6,d0
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_e5:
	mvs.w		a6,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//a6.w * 8
ewf_e6:
	mvs.b		(a0)+,d1
	mvs.w		a6,d0
	lea			0(a1,d0*4),a1
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_e7:
	mvs.w		a6,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//a6.l * 1
ewf_e9:
	move.l		a6,d0
	bra			ewf_full
//a6.l * 6
ewf_eb:
	move.l		a6,d0
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//a6.l * 4
ewf_ed:
	move.l		a6,d0
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//a6.l * 8
ewf_ef:
	move.l		a6,d0
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//usp.w * 1
ewf_f0:
	mvs.b		(a0)+,d1
	move.l		a1,-(a7)
	move.l		usp,a1
	add.l		(a7)+,a1
	add.l		d1,a1
	rts
ewf_f1:
	move.l		a1,-(a7)
	move.l		usp,a1
	mvs.w		a1,d0
	move.l		(a7)+,a1
	bra			ewf_full
//usp.w * 2
ewf_f2:
	mvs.b		(a0)+,d1
	move.l		usp,a1
	mvs.w		a1,d0
	move.l		(a7)+,a1
	lea			0(a1,d0*2),a1
	add.l		d1,a1
	rts
ewf_f3:
	move.l		usp,a1
	move.l		a1,-(a7)
	move.l		usp,a1
	add.l		(a7)+,a1
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//usp.w * 4
ewf_f4:
	mvs.b		(a0)+,d1
	move.l		a1,-(a7)
	move.l		usp,a1
	add.l		(a7)+,a1
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_f5:
	move.l		a1,-(a7)
	move.l		usp,a1
	add.l		(a7)+,a1
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//usp.w * 8
ewf_f6:
	mvs.b		(a0)+,d1
	move.l		a1,-(a7)
	move.l		usp,a1
	add.l		(a7)+,a1
	lea			0(a1,d0*4),a1
	lea			0(a1,d0*4),a1
	add.l		d1,a1
	rts
ewf_f7:
	move.l		a1,-(a7)
	move.l		usp,a1
	add.l		(a7)+,a1
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//usp.l * 1
ewf_f9:
	move.l		a1,-(a7)
	move.l		usp,a1
	add.l		(a7)+,a1
	bra			ewf_full
//usp.l * 7
ewf_fb:
	move.l		a1,-(a7)
	move.l		usp,a1
	add.l		(a7)+,a1
	move.w		ccr,d1
	asl.l		#1,d0
	move.w		d1,ccr
	bra			ewf_full
//usp.l * 4
ewf_fd:
	move.l		a1,-(a7)
	move.l		usp,a1
	add.l		(a7)+,a1
	move.w		ccr,d1
	asl.l		#2,d0
	move.w		d1,ccr
	bra			ewf_full
//usp.l * 8
ewf_ff:
	move.l		a1,-(a7)
	move.l		usp,a1
	add.l		(a7)+,a1
	move.w		ccr,d1
	asl.l		#3,d0
	move.w		d1,ccr
	bra			ewf_full
//-----------------------------------------------------------------------------------
// extension full format rest von ewf
//--------------------------------------------------------------------
ewf_full:
	mvz.b		(a0)+,d1
	mvs.w		ewff_table-.-2(pc,d1*2),d1
	jmp			ewff_table-.-2(pc,d1)
ewff_table:				
	.short		ewff_end-ewff_table,ewff_i0v-ewff_table,ewff_iwv-ewff_table,ewff_ilv-ewff_table	//00
	.short		ewff_end-ewff_table,ewff_i0n-ewff_table,ewff_iwn-ewff_table,ewff_iln-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_i0v-ewff_table,ewff_iwv-ewff_table,ewff_ilv-ewff_table	//10
	.short		ewff_end-ewff_table,ewff_i0n-ewff_table,ewff_iwn-ewff_table,ewff_iln-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_bsw-ewff_table,ewff_w0v-ewff_table,ewff_wwv-ewff_table,ewff_wlv-ewff_table	//20
	.short		ewff_end-ewff_table,ewff_w0n-ewff_table,ewff_wwn-ewff_table,ewff_wln-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_bsl-ewff_table,ewff_l0v-ewff_table,ewff_lwv-ewff_table,ewff_llv-ewff_table	//30
	.short		ewff_end-ewff_table,ewff_l0n-ewff_table,ewff_lwn-ewff_table,ewff_lln-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_mi0-ewff_table,ewff_miw-ewff_table,ewff_mil-ewff_table	//40
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_mi0-ewff_table,ewff_miw-ewff_table,ewff_mil-ewff_table	//50
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_bsw-ewff_table,ewff_wi0-ewff_table,ewff_wiw-ewff_table,ewff_wil-ewff_table	//60
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_bsl-ewff_table,ewff_li0-ewff_table,ewff_liw-ewff_table,ewff_lil-ewff_table	//70
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_i0v-ewff_table,ewff_iwv-ewff_table,ewff_ilv-ewff_table	//80
	.short		ewff_end-ewff_table,ewff_i0n-ewff_table,ewff_iwn-ewff_table,ewff_iln-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_i0v-ewff_table,ewff_iwv-ewff_table,ewff_ilv-ewff_table	//90
	.short		ewff_end-ewff_table,ewff_i0n-ewff_table,ewff_iwn-ewff_table,ewff_iln-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_i0v-ewff_table,ewff_iwv-ewff_table,ewff_ilv-ewff_table	//a0
	.short		ewff_end-ewff_table,ewff_i0n-ewff_table,ewff_iwn-ewff_table,ewff_iln-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_i0v-ewff_table,ewff_iwv-ewff_table,ewff_ilv-ewff_table	//b0
	.short		ewff_end-ewff_table,ewff_i0n-ewff_table,ewff_iwn-ewff_table,ewff_iln-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_mi0-ewff_table,ewff_miw-ewff_table,ewff_mil-ewff_table	//c0
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_mi0-ewff_table,ewff_miw-ewff_table,ewff_mil-ewff_table	//d0
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_mi0-ewff_table,ewff_miw-ewff_table,ewff_mil-ewff_table	//e0
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_mi0-ewff_table,ewff_miw-ewff_table,ewff_mil-ewff_table	//f0
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
	.short		ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table,ewff_end-ewff_table
ewff_end:
	rts
ewff_bsw:
	mvs.w		(a0)+,d1
	add.l		d1,a1
	add.l		d0,a1
	rts
ewff_bsl:
	move.l		(a0)+,d1
	add.l		d1,a1
	add.l		d0,a1
	rts
ewff_i0v:
	add.l		d0,a1
	move.l		(a1),a1
	rts
ewff_iwv:
	add.l		d0,a1
	move.l		(a1),a1
	mvs.w		(a0)+,d0
	add.l		d0,a1
	rts
ewff_ilv:
	add.l		d0,a1
	move.l		(a1),a1
	move.l		(a0)+,d0
	add.l		d0,a1
	rts
ewff_i0n:
	move.l		(a1),a1
	add.l		d0,a1
	rts
ewff_iwn:
	move.l		(a1),a1
	add.l		d0,a1
	mvs.w		(a0)+,d0
	add.l		d0,a1
	rts
ewff_iln:
	move.l		(a1),a1
	add.l		d0,a1
	move.l		(a0)+,d0
	add.l		d0,a1
	rts
ewff_mi0:
	add.l		d0,a1
	rts
ewff_miw:
	mvs.w		(a0)+,d1
	add.l		d0,a1
	add.l		d1,a1
	rts
ewff_mil:
	move.l		(a0)+,d1
	add.l		d0,a1
	add.l		d1,a1
	rts
ewff_wi0:
	mvs.w		(a0)+,d1
	add.l		d1,a1
	add.l		d0,a1
	rts
ewff_wiw:
	mvs.w		(a0)+,d1
	add.l		d1,a1
	mvs.w		(a0)+,d1
	add.l		d0,a1
	add.l		d1,a1
	rts
ewff_wil:
	mvs.w		(a0)+,d1
	add.l		d1,a1
	move.l		(a0)+,d1
	add.l		d0,a1
	add.l		d1,a1
	rts
ewff_li0:
	move.l		(a0)+,d1
	add.l		d1,a1
	add.l		d0,a1
	rts
ewff_liw:
	move.l		(a0)+,d1
	add.l		d1,a1
	mvs.w		(a0)+,d1
	add.l		d0,a1
	add.l		d1,a1
	rts
ewff_lil:
	move.l		(a0)+,d1
	add.l		d1,a1
	move.l		(a0)+,d1
	add.l		d0,a1
	add.l		d1,a1
	rts
ewff_w0v:
	mvs.w		(a0)+,d1
	add.l		d0,a1
	add.l		d1,a1
	move.l		(a1),a1
	rts
ewff_wwv:
	mvs.w		(a0)+,d1
	add.l		d0,a1
	add.l		d1,a1
	move.l		(a1),a1
	mvs.w		(a0)+,d0
	add.l		d0,a1
	rts
ewff_wlv:
	mvs.w		(a0)+,d1
	add.l		d0,a1
	add.l		d1,a1
	move.l		(a1),a1
	move.l		(a0)+,d0
	add.l		d0,a1
	rts
ewff_l0v:
	move.l		(a0)+,d1
	add.l		d0,a1
	add.l		d1,a1
	move.l		(a1),a1
	rts
ewff_lwv:
	move.l		(a0)+,d1
	add.l		d0,a1
	add.l		d1,a1
	move.l		(a1),a1
	mvs.w		(a0)+,d0
	add.l		d0,a1
	rts
ewff_llv:
	move.l		(a0)+,d1
	add.l		d0,a1
	add.l		d1,a1
	move.l		(a1),a1
	move.l		(a0)+,d0
	add.l		d0,a1
	rts
ewff_w0n:
	mvs.w		(a0)+,d1
	add.l		d1,a1
	move.l		(a1),a1
	add.l		d0,a1
	rts
ewff_wwn:
	mvs.w		(a0)+,d1
	add.l		d1,a1
	move.l		(a1),a1
	mvs.w		(a0)+,d0
	add.l		d0,a1
	add.l		d0,a1
	rts
ewff_wln:
	mvs.w		(a0)+,d1
	add.l		d1,a1
	move.l		(a1),a1
	move.l		(a0)+,d0
	add.l		d0,a1
	add.l		d0,a1
	rts
ewff_l0n:
	move.l		(a0)+,d1
	add.l		d1,a1
	move.l		(a1),a1
	add.l		d0,a1
	rts
ewff_lwn:
	move.l		(a0)+,d1
	add.l		d1,a1
	move.l		(a1),a1
	mvs.w		(a0)+,d0
	add.l		d0,a1
	add.l		d0,a1
	rts
ewff_lln:
	move.l		(a0)+,d1
	add.l		d1,a1
	move.l		(a1),a1
	move.l		(a0)+,d0
	add.l		d0,a1
	add.l		d0,a1
	rts


