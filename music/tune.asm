; Cut and paste the data from the line '; --------BEGIN--------'  to the line '; --------END--------'
; into rmt_feat.s
; --------BEGIN--------
; 
FEAT_SFX		= 0
FEAT_GLOBALVOLUMEFADE	= 0		;RMTGLOBALVOLUMEFADE variable
FEAT_NOSTARTINGSONGLINE	= 0
FEAT_INSTRSPEED		= 1
FEAT_CONSTANTSPEED		= 7		;(0 times)
FEAT_COMMAND1		= 1		;(32 times)
FEAT_COMMAND2		= 0		;(0 times)
FEAT_COMMAND3		= 0		;(0 times)
FEAT_COMMAND4		= 0		;(0 times)
FEAT_COMMAND5		= 0		;(0 times)
FEAT_COMMAND6		= 0		;(0 times)
FEAT_COMMAND7SETNOTE		= 0		;(0 times)
FEAT_COMMAND7VOLUMEONLY		= 0		;(0 times)
FEAT_PORTAMENTO		= 0		;(0 times)
FEAT_FILTER		= 1		;(22 times)
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
; Original size: $0253 bytes @ $4000

.export RMT_SONG_DATA
.proc RMT_SONG_DATA
__start:
    .byte "RMT4"
__song_info:
    .byte $40            ; Track length = 64
    .byte $07            ; Song speed
    .byte $01            ; Player Frequency
    .byte $01            ; Format version
; ptrs to tables
__ptrInstrumentTbl:
    .word __InstrumentsTable       ; start + $0010
__ptrTracksTblLo:
    .word __TracksTblLo            ; start + $001a
__ptrTracksTblHi:
    .word __TracksTblHi            ; start + $0020
__ptrSong:
    .word __SongData               ; start + $023b

; List of ptrs to instruments
__InstrumentsTable:
    .word __Instrument_0		; Filter 1                        
    .word __Instrument_1		; Snare drum                      
    .word __Instrument_2		; Hi-Hat closed                   
    .word __Instrument_3		; Echo                            
    .word __Instrument_4		; Hi-Hat open                     

__TracksTblLo:
    .byte <__Track_00,<__Track_01,<__Track_02,<__Track_03,<__Track_04,<__Track_05
__TracksTblHi:
    .byte >__Track_00,>__Track_01,>__Track_02,>__Track_03,>__Track_04,>__Track_05


; Instrument data

__Instrument_0:
    .byte $0c,$0c,$0d,$0d,$00,$00,$40,$00,$00,$00,$00,$00,$00,$ff,$8a,$00
__Instrument_1:
    .byte $0c,$0c,$3a,$3a,$00,$00,$00,$00,$00,$00,$00,$00,$00,$ff,$10,$07
    .byte $dd,$1a,$98,$cc,$1a,$a8,$bb,$1a,$b8,$22,$1a,$cf,$99,$18,$08,$88
    .byte $18,$08,$77,$18,$08,$66,$18,$08,$55,$18,$08,$44,$18,$08,$33,$18
    .byte $08,$22,$18,$08,$22,$18,$08,$11,$18,$08,$00,$10,$00
__Instrument_2:
    .byte $0c,$0c,$16,$16,$00,$00,$00,$00,$00,$00,$00,$00,$00,$ff,$18,$00
    .byte $88,$18,$01,$22,$1a,$02,$00,$10,$00
__Instrument_3:
    .byte $0c,$0c,$49,$0d,$00,$00,$ff,$00,$00,$00,$00,$00,$00,$55,$8c,$00
    .byte $66,$8c,$00,$88,$8c,$00,$bb,$8c,$00,$dd,$8c,$00,$dd,$8a,$00,$ff
    .byte $8a,$00,$bb,$8a,$00,$99,$8a,$00,$77,$8c,$00,$55,$8c,$00,$44,$8c
    .byte $00,$33,$8c,$00,$33,$8c,$00,$44,$8c,$00,$44,$8c,$00,$55,$8c,$00
    .byte $44,$8a,$00,$33,$8a,$00,$22,$8a,$00,$22,$8a,$00
__Instrument_4:
    .byte $0c,$0c,$2e,$2e,$00,$00,$00,$00,$00,$00,$00,$00,$00,$ff,$18,$02
    .byte $bb,$18,$00,$88,$18,$01,$66,$10,$00,$55,$10,$00,$44,$10,$00,$33
    .byte $10,$00,$22,$10,$00,$22,$10,$00,$11,$10,$00,$11,$10,$00,$00,$10
    .byte $00

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
    .byte $3e,$20,$c0,$03,$be,$c3,$03,$7e,$c2,$03,$7e,$c0,$03,$3e,$08,$c3
    .byte $03,$be,$c7,$03,$7e,$c5,$03,$7e,$c3,$03,$3e,$08
__Track_03:
    .byte $c0,$03,$be,$c3,$03,$7e,$c2,$03,$7e,$c0,$03,$3e,$08,$c3,$03,$be
    .byte $c7,$03,$7e,$c5,$03,$7e,$c3,$03,$3e,$08,$c0,$0f,$be,$c3,$0f,$7e
    .byte $c2,$0f,$7e,$c0,$0f,$3e,$08,$c3,$0f,$be,$c7,$0f,$7e,$c5,$0f,$7e
    .byte $c3,$0f,$3e,$08
__Track_04:
    .byte $c0,$0f,$be,$c3,$0f,$7e,$c2,$0f,$7e,$c0,$0f,$3e,$08,$c3,$0f,$be
    .byte $c7,$0f,$7e,$c5,$0f,$7e,$c3,$0f,$3e,$08,$d8,$03,$be,$d3,$03,$7e
    .byte $d1,$03,$7e,$d4,$03,$3e,$08,$cf,$03,$be,$d3,$03,$7e,$d1,$03,$7e
    .byte $cc,$03,$3e,$08
__Track_05:
    .byte $c0,$13,$be,$c0,$13,$c0,$13,$c0,$13,$7e,$c0,$13,$7e,$c0,$13,$be
    .byte $c0,$13,$c0,$13,$c0,$13,$7e,$c0,$13,$7e,$c0,$13,$be,$c0,$13,$c0
    .byte $13,$c0,$13,$7e,$c0,$13,$7e,$c0,$13,$be,$c0,$13,$c0,$13,$c0,$13
    .byte $7e,$c0,$13,$7e,$c0,$13,$be,$c0,$13,$c0,$13,$c0,$13,$7e,$c0,$13
    .byte $7e,$c0,$13,$be,$c0,$13,$c0,$13,$c0,$13,$7e,$c0,$13,$7e,$c0,$13
    .byte $be,$c0,$13,$c0,$13,$c0,$13,$7e,$c0,$13,$7e,$c0,$13

; Song data
__SongData:
__Line_00:  .byte $00,$01,$02,$ff
__Line_01:  .byte $00,$01,$03,$ff
__Line_02:  .byte $00,$01,$04,$ff
__Line_03:  .byte $00,$01,$04,$05
__Line_04:  .byte $00,$01,$03,$05
__Line_05:  .byte $fe,$03,<__Line_03,>__Line_03

.endproc

