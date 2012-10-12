/*******************************************************/
// allgemeine macros
/*******************************************************/
.text
wait_pll: .macro
wait1_pll\@:
		tst.w	(a1)
		bmi		wait1_pll\@
		rts
.endm
