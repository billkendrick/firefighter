/*
  Firefighter core `main()` loop that calls other loops
  (title screen, help screen, and the game).

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2023-09-25
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

extern char main_stick;
extern char level;

extern unsigned long int score;
extern unsigned long int high_score;
extern char high_score_name[4];

/* Main loop! */
void main(void) {
  char cmd;
  signed char pos;

  /* Set default high score */
  set_default_high_score();

#ifdef DISK
  /* Load high score table from disk */
  load_high_scores();
#endif

  /* Set default config */
  set_default_config();

#if defined(DISK) || defined(FUJINET)
  /* Load saved config (disk & fujinet) and high score (fujinet) */
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
        show_high_score_table(-1);
      }
#endif
    } while (cmd != CMD_PLAY);

    /* Play the game! */
    start_game();

    /* Handle high score initials entry */
    pos = register_high_score();
    if (pos != -1) {
#ifdef DISK
      save_high_scores();
      show_high_score_table(pos);
#endif
#ifdef FUJINET
      save_config();
#endif
    }
  } while(1);
}
