/*
  Firefighter core `main()` loop that calls other loops
  (title screen, help screen, and the game).

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2023-08-27
*/

#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include "title.h"
#include "game.h"
#include "score.h"
#include "config.h"

#ifdef DISK
#include "help.h"
#endif

extern unsigned char scr_mem[];
unsigned char * dlist = (scr_mem + 960);
extern unsigned long int high_score;
extern char high_score_name[4];
char main_stick;
char level;

/* Main loop! */
void main(void) {
  char cmd;

  /* Set default high score */
  set_default_high_score();

  /* Set default config */
  main_stick = STICK_LEFT;
  level = 1;

#ifdef DISK
  /* Load saved config from disk */
  load_config();
#endif

  do {
    do {
      /* Show title screen */
      cmd = show_title();

#ifdef DISK
      if (cmd == CMD_HELP) {
        /* Show help screen */
        show_help();
      } else if (cmd == CMD_HIGHSCORES) {
        /* Show high score screen */
        show_high_score_table();
      }
#endif
    } while (cmd != CMD_PLAY);

    /* Play the game! */
    start_game();
  } while(1);
}
