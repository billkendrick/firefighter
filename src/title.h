/*
  Firefighter title screen

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  https://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2026-10-11
*/

#ifndef _TITLE_H
#define _TITLE_H

#define CMD_PLAY 0
#ifdef DISK
#define CMD_HELP 1
#define CMD_HIGHSCORES 2
#endif

char show_title(void);

#endif

