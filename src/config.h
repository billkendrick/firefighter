/*
  Firefighter config settings, and
  file (disk version) or FujiNet App Key I/O routines

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-22 - 2023-09-24
*/

#ifndef _CONFIG_H
#define _CONFIG_H

/* Which joystick is movement vs. spray (`main_stick`) */
#define STICK_LEFT 0
#define STICK_RIGHT 1

void set_default_config(void);

#if defined(DISK) || defined(FUJINET)
void load_config(void);
void save_config(void);
#endif

#endif

