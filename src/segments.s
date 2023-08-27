	.setcpu		"6502"

	.export		_scr_mem
.segment	"SCRMEM"
_scr_mem:

	.export		_font1_data
.segment	"FONT1"
_font1_data:
	.incbin "fonts/fire1.fnt"
	.code

	.export		_font2_data
.segment	"FONT2"
_font2_data:
	.incbin "fonts/fire2.fnt"
	.code

	.export		_levels_data
.segment	"LEVELS"
_levels_data:
	.incbin "data/levels_cmp.dat"
	.code
