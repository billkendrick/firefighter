/*
  Firefighter title screen

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2023-08-27
*/

#ifndef _TITLE_H
#define _TITLE_H

enum {
  CMD_PLAY,
#ifdef DISK
  CMD_HELP,
  CMD_HIGHSCORES,
#endif
};

char show_title(void);

#endif

