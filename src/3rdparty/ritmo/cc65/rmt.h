/* rmt.h - the RMT player from C (cc65), see README.md */
#ifndef RMT_H
#define RMT_H

/* Starts a module from its first song line: the module is the label of a "Relocatable ASM for RMTPlayer" export in
   the CA65 format. Returns the instrument speed (the calls of the player per frame, 1..4). */
unsigned char __fastcall__ rmt_init(const void *module);

/* The player on the deferred VBI (it plays by itself, every frame), and off again with the POKEY silent. */
void rmt_vbi_on(void);
void rmt_vbi_off(void);

/* Frames played since rmt_vbi_on() (50 a second on PAL, 60 on NTSC). */
extern volatile unsigned int rmt_frames;

#endif
