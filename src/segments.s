	.setcpu		"6502"

	.export		_scr_mem
.segment	"SCRMEM"
_scr_mem:

	.export		_fonttext_data
.segment	"FONTTEXT"
_fonttext_data:
	.incbin "fonts/firetext.fnt"
	.code

	.export		_fontshap_data
.segment	"FONTSHAP"
_fontshap_data:
	.incbin "fonts/fireshap.fnt"
	.code

	.export		_levels_data
.segment	"LEVELS"
_levels_data:
	.incbin "data/levels_cmp.dat"
	.code
