/*
  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/

  2023-08-13 - 2023-08-20
*/

#include <peekpoke.h>
#include <string.h>
#include "draw_text.h"


extern unsigned char scr_mem[];

void draw_text(char * str, unsigned char * dest) {
  unsigned char ch;
  unsigned int i;

  for (i = 0; str[i] != '\0'; i++) {
    ch = str[i];

    if (ch < 32) {
      ch = ch + 64;
    } else if (ch < 96) {
      ch = ch - 32;
    }

    POKE((unsigned int) (dest + i), ch);
  }
}

void draw_number(unsigned long int n, int digits, unsigned char * dest) {
  do {
    POKE(dest + digits - 1, (n % 10) + 16);
    n = n / 10;
    digits--;
  } while (digits > 0);
}

