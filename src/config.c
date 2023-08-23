/*
  Firefighter Config File (disk version) routines

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-22 - 2023-08-22
*/

#include <atari.h>
#include <string.h>
#include <stdio.h>
#include "config.h"

extern char level;
extern char main_stick;

/* (Attempt to) load config from disk */
void load_config() {
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
