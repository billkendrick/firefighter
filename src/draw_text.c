/*
  Firefighter text drawing routines

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2025-01-18
*/

#include <peekpoke.h>
#include <string.h>
#include "draw_text.h"


extern unsigned char scr_mem[];

/* Draw some text on the screen.

   @param char * str - The NUL-terminated ('\0') string to write
   @param unsigned char * dest - The destination in memory
     (expected to be within src_mem[]!!!)
*/
void draw_text(char * str, unsigned char * dest) {
  unsigned char ch, ch2;
  unsigned int i;

  for (i = 0; str[i] != '\0'; i++) {
    ch = str[i];
    ch2 = ch & 127;

    if (ch2 < 32) {
      ch |= 64;
    } else if (ch2 < 96) {
      ch = ch - 32;
    }

    *dest = ch;
    dest++;
  }
}

/* Draw some text on the screen in inverse-video.

   @param char * str - The NUL-terminated ('\0') string to write
   @param unsigned char * dest - The destination in memory
     (expected to be within src_mem[]!!!)
*/
void draw_text_inv(char * str, unsigned char * dest) {
  unsigned char ch, ch2;
  unsigned int i;

  for (i = 0; str[i]; i++) {
    ch = str[i];
    ch2 = ch & 127;

    if (ch2 < 32) {
      ch |= 64;
    } else if (ch2 < 96) {
      ch = ch - 32;
    }

    *dest = (ch | 128);
    dest++;
  }
}

/* Draw a zero-padded decimal number on the screen.

   @param unsigned long int n - The number to draw
   @param signed char digits - How many digits to show (will be zero-padded)
   @unsigned char * dest - The destination in memory
     (expected to within src_mem[]!!!)
*/
void draw_number(unsigned long int n, signed char digits, unsigned char * dest) {
  unsigned char * ddest;
  ddest = dest + digits - 1;
  do {
    *ddest = (n % 10) | 16;

    digits--;
    if (digits) {
      n = (n * 0xCCCD) >> 19; /* n = n / 10 */
      ddest--;
    }
  } while (digits);
}
