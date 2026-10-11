/*
  Firefighter text drawing routines

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  https://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2026-10-11
*/

void draw_text(char * str, unsigned char * dest);
void draw_text_inv(char * str, unsigned char * dest);
void draw_number(unsigned long int n, signed char digits, unsigned char * dest);
