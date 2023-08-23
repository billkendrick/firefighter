/*
  Firefighter core `main()` loop that calls other loops
  (title screen, help screen, and the game).

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2023-08-22
*/

#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include "title.h"
#include "game.h"

#ifdef DISK
#include "help.h"
#include "config.h"
#endif

extern unsigned char scr_mem[];
unsigned char * dlist = (scr_mem + 960);
unsigned long int high_score;
char high_score_name[4];
char main_stick;
char level;

/* Main loop! */
void main(void) {
  char want_help;

  /* Set default high score */
  high_score = 1031;
  strcpy(high_score_name, "BJK");

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
      want_help = show_title();

#ifdef DISK
      if (want_help) {
        /* Show help screen */
        show_help();
      }
#endif
    } while (want_help);

    /* Play the game! */
    start_game();
  } while(1);
}
