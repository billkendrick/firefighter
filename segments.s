	.setcpu		"6502"

	.export		_scr_mem
.segment	"SCRMEM"
_scr_mem:

	.export		_font1_data
.segment	"FONT1"
_font1_data:
	.incbin "fire1.fnt"
	.code

	.export		_font2_data
.segment	"FONT2"
_font2_data:
	.incbin "fire2.fnt"
	.code
