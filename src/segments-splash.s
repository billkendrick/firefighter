	.setcpu		"6502"

	.export		_scr_mem
.segment	"SCRMEM"
_scr_mem:

	.export		_rmt_module
.import		RMT_SONG_DATA
_rmt_module = RMT_SONG_DATA
