; mads.inc - the Atari binary file of MADS/XASM with ca65 (see README.md)
;
; MADS writes $FFFF, then for each "org" a segment header (start, end) and its bytes; a segment without bytes is
; left out, and "opt h-" writes the bytes without headers. Here:
;
;   ORG addr     starts a segment at addr      (org addr, also org *+n)
;   OPT_H 0/1    headers off/on                (opt h- / opt h+)
;   RUN addr     the run address segment       (run addr: $02E0-$02E1)
;   INI addr     the init address segment      (ini addr: $02E2-$02E3)
;   OBX_END      ends the last segment, at the end of the main source
;
; Whether a segment is empty is known only at its end, but its header comes first: the build assembles twice.
; The first time each empty segment prints "OBX_EMPTY <n>"; the second time obx_empty.inc (from those lines)
; defines OBX_EMPTY_<n> and the header of that segment is left out. Assemble with:
;   ca65 -i -t none --feature labels_without_colons ...   (see the Makefile)

.ifndef MADS_INC
MADS_INC = 1

.include "obx_empty.inc"

OBX_HEADERS .set 1      ; opt h+
OBX_FFFF    .set 0      ; $FFFF written
OBX_SEG     .set 0      ; number of the current segment, 0 = none
OBX_OPEN    .set 0      ; the current segment has a header waiting for its end

; end of the current segment: OBX_E_<n> = its end address + 1, OBX_EMPTY <n> printed if it has no bytes
.macro OBX_CLOSE
    .if OBX_OPEN
        .ident(.sprintf("OBX_E_%d", OBX_SEG)) = *
        .if .ident(.sprintf("OBX_E_%d", OBX_SEG)) = .ident(.sprintf("OBX_S_%d", OBX_SEG))
            .out .sprintf("OBX_EMPTY %d", OBX_SEG)
        .endif
        OBX_OPEN .set 0
    .endif
.endmacro

; the address first: it may be relative to * (org *+n), and the header moves *
; an org at the end of a segment with bytes continues it, as in MADS (no new header)
.macro ORG addr
    OBX_ADDR .set addr
    .if OBX_OPEN
        .if OBX_ADDR = * .and * <> .ident(.sprintf("OBX_S_%d", OBX_SEG))
            .exitmacro
        .endif
    .endif
    OBX_CLOSE
    OBX_SEG .set OBX_SEG + 1
    .if OBX_HEADERS
        .ifndef .ident(.sprintf("OBX_EMPTY_%d", OBX_SEG))
            .if !OBX_FFFF
                .word $FFFF
                OBX_FFFF .set 1
            .endif
            .word OBX_ADDR, .ident(.sprintf("OBX_E_%d", OBX_SEG)) - 1
        .endif
        OBX_OPEN .set 1
    .endif
    .org OBX_ADDR
    .ident(.sprintf("OBX_S_%d", OBX_SEG)) = *
.endmacro

; $FFFF comes only at the start of the file: after bytes without headers (opt h-) the source writes it itself
; when it wants it (the SAP header of sap.asm ends with dta $FF,$FF)
.macro OPT_H on
    OBX_CLOSE
    OBX_HEADERS .set on
    .if !on
        OBX_FFFF .set 1
    .endif
.endmacro

.macro RUN addr
    ORG $02E0
    .word addr
.endmacro

.macro INI addr
    ORG $02E2
    .word addr
.endmacro

.macro OBX_END
    OBX_CLOSE
.endmacro

.endif
