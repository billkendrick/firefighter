/*
  Firefighter Config File (disk version) routines

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-22 - 2023-09-24
*/

#include <atari.h>
#include <string.h>
#include <stdio.h>
#include "config.h"

#ifdef FUJINET
#include "app_key.h"
#endif

char level;
char main_stick;

/* Set default config (if we cannot load) */
void set_default_config(void) {
  main_stick = STICK_LEFT;
  level = 1;
}

#ifdef DISK

/* (Attempt to) load config from disk */
void load_config(void) {
  FILE * fi;

  fi = fopen("FIREFITE.CFG", "rb");
  if (fi != NULL) {
    level = (char) fgetc(fi);
    main_stick = (char) fgetc(fi);
    fclose(fi);
  }
}

/* (Attempt to) save config to disk */
void save_config() {
  FILE * fi;

  fi = fopen("FIREFITE.CFG", "wb");
  if (fi != NULL) {
    fputc((int) level, fi);
    fputc((int) main_stick, fi);
    fclose(fi);
  }
}

#endif /* DISK */


#ifdef FUJINET

/* (Attempt to) load config and high score from FujiNet App Key storage */
void load_config(void) {
  read_settings();
}

/* (Attempt to) save config and high score to FujiNet App Key storage */
void save_config() {
  write_settings();
}

#endif /* FUJINET */
