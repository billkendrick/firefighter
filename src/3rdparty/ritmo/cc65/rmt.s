; rmt.s - the RMT player from C (cc65): rmt_init(), and the player on the deferred VBI (see README.md).
;
; The player (Patch16, asm/ca65/Patch-16/rmtplayr.s) is assembled at its own address by the Makefile and loaded from
; the same XEX; player.inc has its address, RMT_PLAYER, with three vectors: +0 init (X/Y = module, A = song line;
; returns the instrument speed), +3 play, +9 silence. Every frame the VBI calls the player as many times as the
; instrument speed, back to back, as the XEX export of RITMO does.

        .include "player.inc"

        .export _rmt_init, _rmt_vbi_on, _rmt_vbi_off, _rmt_frames

; The global volume fade of a player assembled with FEAT_GLOBALVOLUMEFADE=1: player.inc then also has
; RMT_GLOBALVOLUMEFADE, the address of the byte the player subtracts from every volume (the operand of a SBC in
; the player's code). For C it is the variable rmt_global_volume_fade, at that address.
.ifdef RMT_GLOBALVOLUMEFADE
_rmt_global_volume_fade = RMT_GLOBALVOLUMEFADE
        .export _rmt_global_volume_fade
.endif

SETVBV  = $E45C                 ; OS: set a VBI vector (A = 7 deferred, Y/X = address)
XITVBV  = $E462                 ; OS: the end of the deferred VBI
VVBLKD  = $0224                 ; the deferred VBI vector

rmt_play    = RMT_PLAYER + 3
rmt_silence = RMT_PLAYER + 9

        .segment "BSS"
_rmt_frames:    .res 2          ; frames played
ispeed:         .res 1          ; player calls per frame (instrument speed, 1..4)
ticks:          .res 1
oldvbd:         .res 2          ; the VBI vector before rmt_vbi_on()
vbion:          .res 1

        .segment "CODE"

; unsigned char __fastcall__ rmt_init(const void *module)
_rmt_init:
        pha                     ; A/X = module lo/hi -> X/Y for the player
        txa
        tay
        pla
        tax
        lda #0                  ; from song line 0
        jsr RMT_PLAYER
        cmp #1                  ; 1..4, the RMT range
        bcs @lo
        lda #1
@lo:    cmp #5
        bcc @hi
        lda #4
@hi:    sta ispeed
        ldx #0
        rts

; void rmt_vbi_on(void)
_rmt_vbi_on:
        lda vbion
        bne @done
        lda VVBLKD
        sta oldvbd
        lda VVBLKD+1
        sta oldvbd+1
        ldy #<vbi
        ldx #>vbi
        lda #7                  ; 7 = deferred VBI
        jsr SETVBV
        inc vbion
@done:  rts

; void rmt_vbi_off(void)
_rmt_vbi_off:
        lda vbion
        beq @done
        ldy oldvbd
        ldx oldvbd+1
        lda #7
        jsr SETVBV
        lda #0
        sta vbion
        jsr rmt_silence
@done:  rts

vbi:
        inc _rmt_frames
        bne @nohi
        inc _rmt_frames+1
@nohi:  lda ispeed
        sta ticks
@l:     jsr rmt_play
        dec ticks
        bne @l
        jmp XITVBV
