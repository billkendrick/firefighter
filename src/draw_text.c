/*
  Firefighter text drawing routines

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2024-07-17
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
  unsigned char ch;
  unsigned int i;

  for (i = 0; str[i] != '\0'; i++) {
    ch = str[i];

    if (ch < 32 || (ch >= 128 && ch < 128 + 32)) {
      ch = ch + 64;
    } else if (ch < 96 || (ch >= 128 && ch < 128 + 96)) {
      ch = ch - 32;
    }

    POKE((unsigned int) (dest + i), ch);
  }
}

/* Draw some text on the screen in inverse-video.

   @param char * str - The NUL-terminated ('\0') string to write
   @param unsigned char * dest - The destination in memory
     (expected to be within src_mem[]!!!)
*/
void draw_text_inv(char * str, unsigned char * dest) {
  unsigned char ch;
  unsigned int i;

  for (i = 0; str[i] != '\0'; i++) {
    ch = str[i];

    if (ch < 32 || (ch >= 128 && ch < 128 + 32)) {
      ch = ch + 64;
    } else if (ch < 96 || (ch >= 128 && ch < 128 + 96)) {
      ch = ch - 32;
    }

    POKE((unsigned int) (dest + i), ch + 128);
  }
}

/* Draw a zero-padded decimal number on the screen.

   @param unsigned long int n - The number to draw
   @param signed char digits - How many digits to show (will be zero-padded)
   @unsigned char * dest - The destination in memory
     (expected to within src_mem[]!!!)
*/
void draw_number(unsigned long int n, signed char digits, unsigned char * dest) {
  do {
    POKE(dest + digits - 1, (n % 10) + 16);
    n = n / 10;
    digits--;
  } while (digits > 0);
}
