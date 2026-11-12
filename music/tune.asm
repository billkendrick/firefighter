; Cut and paste the data from the line '; --------BEGIN--------'  to the line '; --------END--------'
; into rmt_feat.s
; --------BEGIN--------
; 
FEAT_SFX		= 0
FEAT_GLOBALVOLUMEFADE	= 0		;RMTGLOBALVOLUMEFADE variable
FEAT_NOSTARTINGSONGLINE	= 0
FEAT_INSTRSPEED		= 1
FEAT_CONSTANTSPEED		= 16		;(0 times)
FEAT_COMMAND1		= 1		;(20 times)
FEAT_COMMAND2		= 0		;(0 times)
FEAT_COMMAND3		= 0		;(0 times)
FEAT_COMMAND4		= 0		;(0 times)
FEAT_COMMAND5		= 0		;(0 times)
FEAT_COMMAND6		= 0		;(0 times)
FEAT_COMMAND7SETNOTE		= 0		;(0 times)
FEAT_COMMAND7VOLUMEONLY		= 0		;(0 times)
FEAT_PORTAMENTO		= 0		;(0 times)
FEAT_FILTER		= 0		;(0 times)
FEAT_FILTERG0L		= 0		;(0 times)
FEAT_FILTERG1L		= 0		;(0 times)
FEAT_FILTERG0R		= 0		;(0 times)
FEAT_FILTERG1R		= 0		;(0 times)
FEAT_BASS16		= 0		;(0 times)
FEAT_BASS16G1L		= 0		;(0 times)
FEAT_BASS16G3L		= 0		;(0 times)
FEAT_BASS16G1R		= 0		;(0 times)
FEAT_BASS16G3R		= 0		;(0 times)
FEAT_VOLUMEONLYG0L		= 0		;(0 times)
FEAT_VOLUMEONLYG2L		= 0		;(0 times)
FEAT_VOLUMEONLYG3L		= 0		;(0 times)
FEAT_VOLUMEONLYG0R		= 0		;(0 times)
FEAT_VOLUMEONLYG2R		= 0		;(0 times)
FEAT_VOLUMEONLYG3R		= 0		;(0 times)
FEAT_TABLETYPE		= 0		;(0 times)
FEAT_TABLEMODE		= 0		;(0 times)
FEAT_TABLEGO		= 0		;(0 times)
FEAT_AUDCTLMANUALSET		= 0		;(0 times)
FEAT_VOLUMEMIN		= 0		;(0 times)
FEAT_EFFECTVIBRATO		= 0		;(0 times)
FEAT_EFFECTFSHIFT		= 0		;(0 times)
; --------END--------
; RMT4 file exported as relocatable source code
; Original size: $00d1 bytes @ $4000

.export RMT_SONG_DATA
.proc RMT_SONG_DATA
__start:
    .byte "RMT4"
__song_info:
    .byte $40            ; Track length = 64
    .byte $10            ; Song speed
    .byte $01            ; Player Frequency
    .byte $01            ; Format version
; ptrs to tables
__ptrInstrumentTbl:
    .word __InstrumentsTable       ; start + $0010
__ptrTracksTblLo:
    .word __TracksTblLo            ; start + $0016
__ptrTracksTblHi:
    .word __TracksTblHi            ; start + $001a
__ptrSong:
    .word __SongData               ; start + $00c9

; List of ptrs to instruments
__InstrumentsTable:
    .word   $0000		; Bass tone 1                     
    .word __Instrument_1		; Snare drum                      
    .word __Instrument_2		; Hi-Hat closed                   

__TracksTblLo:
    .byte <__Track_00,<__Track_01,<__Track_02,<__Track_03
__TracksTblHi:
    .byte >__Track_00,>__Track_01,>__Track_02,>__Track_03


; Instrument data

__Instrument_1:
    .byte $0c,$0c,$3a,$3a,$00,$00,$00,$00,$00,$00,$00,$00,$00,$ff,$10,$07
    .byte $dd,$1a,$98,$cc,$1a,$a8,$bb,$1a,$b8,$22,$1a,$cf,$99,$18,$08,$88
    .byte $18,$08,$77,$18,$08,$66,$18,$08,$55,$18,$08,$44,$18,$08,$33,$18
    .byte $08,$22,$18,$08,$22,$18,$08,$11,$18,$08,$00,$10,$00
__Instrument_2:
    .byte $0c,$0c,$16,$16,$00,$00,$00,$00,$00,$00,$00,$00,$00,$ff,$18,$00
    .byte $88,$18,$01,$22,$1a,$02,$00,$10,$00

; Track data
__Track_00:
    .byte $c2,$0b,$fe,$c2,$0b,$7e,$c2,$0b,$c2,$0b,$c2,$0b,$fe,$c2,$0b,$fe
    .byte $c2,$0b,$fe,$c2,$0b,$7e,$c2,$0b,$c2,$0b,$c2,$0b,$fe,$c2,$0b,$fe
    .byte $c2,$0b,$fe,$c2,$0b,$7e,$c2,$0b,$c2,$0b,$c2,$0b,$fe,$c2,$0b,$fe
    .byte $c2,$0b,$fe,$c2,$0b,$7e,$c2,$0b,$c2,$0b,$c2,$0b,$fe,$c2,$0b,$fe
__Track_01:
    .byte $3e,$0e,$c2,$07,$3e,$0f,$c2,$07,$3e,$0f,$c2,$07,$3e,$0f,$c2,$07
    .byte $7e
__Track_02:
    .byte $3e,$40
__Track_03:
    .byte $3e,$40

; Song data
__SongData:
__Line_00:  .byte $00,$01,$02,$03
__Line_01:  .byte $fe,$00,<__Line_00,>__Line_00

.endproc

