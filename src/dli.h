/*
  Firefighter Display List Interrupt routines

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  https://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2026-10-11
*/

#define ANIM_ENABLE 8
#define ANIM_DISABLE 0

extern void * OLDVEC;

void dli(void);
void dli_vbi(void);

#ifdef DISK
void hs_dli(void);
#endif

