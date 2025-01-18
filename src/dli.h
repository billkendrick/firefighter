/*
  Firefighter Display List Interrupt routines

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2025-01-17
*/

#define ANIM_ENABLE 8
#define ANIM_DISABLE 0

extern void * OLDVEC;

void dli(void);
void dli_vbi(void);
