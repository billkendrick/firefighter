/*
  Firefighter title screen

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2025-01-15
*/

#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include "title.h"
#include "shapes.h"
#include "draw_text.h"
#include "dli.h"
#include "game.h"
#include "score.h"
#include "config.h"

extern unsigned char font1_data[];
extern unsigned char scr_mem[];
extern unsigned char levels_data[];
extern unsigned char * dlist;

extern char level;
extern char main_stick;
extern unsigned long int high_score;
extern char high_score_name[4];

void show_controls(void);

#define LINE_BY_1 60
#define LINE_BY_2 (LINE_BY_1 + 20)
#define LINE_HELP_1 (LINE_BY_2 + 20)
#define LINE_HELP_2 (LINE_HELP_1 + 60)
#define LINE_HELP_3 (LINE_HELP_2 + 60)
#define LINE_CONTROLS (LINE_HELP_3 + 60)
#define LINE_START (LINE_CONTROLS + 120)
#define LINE_LEVEL (LINE_START + 40)
#define LINE_HIGH (LINE_LEVEL + 40)
#define LINE_VERSION (LINE_HIGH + 40)

/* Title screen display list */
void title_dlist = {
  DL_BLK8,
  DL_BLK8,
  DL_BLK1,

  DL_LMS(DL_GRAPHICS0), // NBS Presents
  scr_mem,
  DL_BLK2,

  DL_GRAPHICS2, // FIREFIGHTER!
  DL_BLK2,
  DL_GRAPHICS1, // Bill Kendrick
  DL_DLI(DL_GRAPHICS1), // <animated credits line>

  DL_GRAPHICS0, // Item Row 1 Desc
  DL_DLI(DL_BLK2),
  DL_GRAPHICS1, // Item Row 1 Images
  DL_LMS(DL_GRAPHICS1),
  (scr_mem + LINE_HELP_1 + 40),
  DL_BLK6,

  DL_GRAPHICS0, // Item Row 2 Desc
  DL_DLI(DL_BLK2),
  DL_GRAPHICS1, // Item Row 2 Images
  DL_LMS(DL_GRAPHICS1),
  (scr_mem + LINE_HELP_2 + 40),
  DL_BLK6,

  DL_GRAPHICS0, // Item Row 3 Desc
  DL_DLI(DL_BLK2),
  DL_GRAPHICS1, // Item Row 3 Images
  DL_LMS(DL_GRAPHICS1),
  (scr_mem + LINE_HELP_3 + 40),
  DL_BLK6,

  DL_BLK2,

  DL_GRAPHICS0, // Game controls...
  DL_GRAPHICS0,
  DL_GRAPHICS0,

  DL_BLK1,

  DL_GRAPHICS0, // Title screen controls/options...
  DL_GRAPHICS0,

  DL_BLK4,
  DL_GRAPHICS0, // High score

  DL_BLK1,
  DL_GRAPHICS0, // Version

  DL_JVB,
  &dlist
};

char * additional_levels[3] = {
  "additional levels by",
  "edward",
  "",
};


/* Set up and display title screen; title screen loop
   @return char - what the user wants to do:
     * CMD_PLAY play the game
     * CMD_HELP show help screen (disk only)
     * CMD_HIGHSCORES show high score table (disk only)
*/
char show_title(void) {
  unsigned char siren_ctr1, siren_ctr2, siren_pitch, siren_doppler, siren_doppler_dir, honk;
  unsigned int select_down, level_pos, hs_pos, select_down_wait;
  unsigned int levels_by_wait;
  unsigned char levels_by_idx;
  unsigned char option_down, cmd;

  /* FIXME: Screen setup could be moved to a function -bjk 2023.08.22 */

  OS.sdmctl = 0;

  bzero(scr_mem, 1024);

  /* Generate a display list... */
  memcpy(dlist, &title_dlist, sizeof(title_dlist));
  OS.sdlst = dlist;

  /* Set character set */
  OS.chbas = (unsigned char) ((unsigned int) font1_data / 256);
  POKE(0x600, OS.chbas + 2);

  /* Set color palette */
  OS.color0 = 0x52;
  OS.color1 = 0xCA;
  OS.color2 = 0x02;
  OS.color3 = 0x46;
  OS.color4 = 0x02;

  POKE(0x602, 0x20); // fire yellow
  POKE(0x603, 0xCA); // medium green
  POKE(0x604, 0x86); // medium blue

  POKE(0x605, 0x0F); // enable fire flicker

  POKE(0x606, 0x04); // enable animation

  /* Enable DLI rotuine */
  ANTIC.nmien = NMIEN_VBI;
  while (ANTIC.vcount < 124);
  OS.vdslst = (void *) dli;
  ANTIC.nmien = NMIEN_VBI | NMIEN_DLI;

  /* Enable VBI routine to start each frame with dli #1 */
  OLDVEC = OS.vvblkd;
  OS.critic = 1;
  OS.vvblkd = (void *) dli_vbi;
  OS.critic = 0;

  /* Title & Credits */
  draw_text("NEW BREED SOFTWARE PRESENTS:", scr_mem + 6);
  draw_text("@ FIREFIGHTER! @", scr_mem + 40 + 2);

  draw_text("bill kendrick", scr_mem + LINE_BY_1 + 1);
  /* "2024" */
  POKE(scr_mem + LINE_BY_1 + 15, 2 + 16 + 64);
  POKE(scr_mem + LINE_BY_1 + 16, 0 + 16 + 64);
  POKE(scr_mem + LINE_BY_1 + 17, 2 + 16 + 64);
  POKE(scr_mem + LINE_BY_1 + 18, 4 + 16 + 64);

  draw_text(additional_levels[0], scr_mem + LINE_BY_2);

  /* Help section 1 */
  draw_text("LEAD TO EXIT. PUT OUT FIRE. WALL. PIPES.", scr_mem + LINE_HELP_1 + 0);

  POKE(scr_mem + (LINE_HELP_1 + 40) + 0, CIVILIAN);
  POKE(scr_mem + (LINE_HELP_1 + 40) + 1, CIVILIAN);
  POKE(scr_mem + (LINE_HELP_1 + 40) + 2, FIREFIGHTER_RIGHT);

  POKE(scr_mem + (LINE_HELP_1 + 40) + 3, EXIT1);
  POKE(scr_mem + (LINE_HELP_1 + 40) + 4, EXIT2);
  POKE(scr_mem + (LINE_HELP_1 + 40) + 5, DOOR);

  POKE(scr_mem + (LINE_HELP_1 + 40) + 7, FIRE_LG);
  POKE(scr_mem + (LINE_HELP_1 + 40) + 8, FIRE_MD);
  POKE(scr_mem + (LINE_HELP_1 + 40) + 9, FIRE_SM);

  POKE(scr_mem + (LINE_HELP_1 + 40) + 10, 'e' + 64);
  POKE(scr_mem + (LINE_HELP_1 + 40) + 11, 'g' + 64);
  POKE(scr_mem + (LINE_HELP_1 + 40) + 12, FIREFIGHTER_LEFT);

  POKE(scr_mem + (LINE_HELP_1 + 40) + 14, WALL);
  POKE(scr_mem + (LINE_HELP_1 + 40) + 15, WALL);

  POKE(scr_mem + (LINE_HELP_1 + 40) + 17, PIPE_DOWN_RIGHT);
  POKE(scr_mem + (LINE_HELP_1 + 40) + 18, PIPE_LEFT_RIGHT);
  POKE(scr_mem + (LINE_HELP_1 + 40) + 19, PIPE_UP_LEFT);

  /* Help section 2 */
  draw_text("PUSH CRATES&OIL. AX BREAKS CRATES&PIPES.", scr_mem + LINE_HELP_2 + 0);

  POKE(scr_mem + (LINE_HELP_2 + 40) + 2, FIREFIGHTER_RIGHT);
  POKE(scr_mem + (LINE_HELP_2 + 40) + 3, CRATE);
  POKE(scr_mem + (LINE_HELP_2 + 40) + 6, OIL);

  POKE(scr_mem + (LINE_HELP_2 + 40) + 9, AX);

  POKE(scr_mem + (LINE_HELP_2 + 40) + 14, CRATE_BROKEN);

  POKE(scr_mem + (LINE_HELP_2 + 40) + 17, GASLEAK_LEFT);
  POKE(scr_mem + (LINE_HELP_2 + 40) + 18, PIPE_BROKEN_UP_DOWN);
  POKE(scr_mem + (LINE_HELP_2 + 40) + 19, GASLEAK_RIGHT);


  /* Help section 3 */
  draw_text("TURN VALVE. OIL&LEAKS EXPLODE! PUSH EXIT", scr_mem + LINE_HELP_3 + 0);

  POKE(scr_mem + (LINE_HELP_3 + 40) + 1, PIPE_LEFT_RIGHT);
  POKE(scr_mem + (LINE_HELP_3 + 40) + 2, VALVE_OPEN);
  POKE(scr_mem + (LINE_HELP_3 + 40) + 3, PIPE_LEFT_RIGHT);

  POKE(scr_mem + (LINE_HELP_3 + 40) + 7, FIRE_LG);
  POKE(scr_mem + (LINE_HELP_3 + 40) + 8, FIRE_LG);
  POKE(scr_mem + (LINE_HELP_3 + 40) + 9, OIL);
  POKE(scr_mem + (LINE_HELP_3 + 40) + 10, FIRE_LG);

  POKE(scr_mem + (LINE_HELP_3 + 40) + 16, FIREFIGHTER_RIGHT);
  POKE(scr_mem + (LINE_HELP_3 + 40) + 17, DOOR);

  /* Control instructions */
  show_controls();
  draw_text("TO SPRAY.", scr_mem + LINE_CONTROLS + 80 + 16);

  /* Title screen control instructions */
  draw_text("START/FIRE: BEGIN - OPTION: SWAP STICKS", scr_mem + LINE_START + 0);
  draw_text_inv("START", scr_mem + LINE_START + 0);
  draw_text_inv("FIRE", scr_mem + LINE_START + 6);
  draw_text_inv("OPTION", scr_mem + LINE_START + 20);

#ifdef DISK
  draw_text("SELECT: STARTING LEVEL -- - HELP/?: INFO", scr_mem + LINE_LEVEL);
  draw_text_inv("SELECT", scr_mem + LINE_LEVEL);
  draw_text_inv("HELP", scr_mem + LINE_LEVEL + 28);
  draw_text_inv("?", scr_mem + LINE_LEVEL + 33);
  level_pos = LINE_LEVEL + 23; /* FIXME #define */
#else
  draw_text("SELECT: STARTING LEVEL --", scr_mem + LINE_LEVEL + 7);
  draw_text_inv("SELECT", scr_mem + LINE_LEVEL + 7);
  level_pos = LINE_LEVEL + 30; /* FIXME #define */
#endif

  draw_number(level, 2, scr_mem + level_pos);

  /* High score */
#ifdef DISK
  draw_text("HIGH SCORE: ------ --- - H: HIGH SCORES", scr_mem + LINE_HIGH + 0);
  draw_text_inv("H", scr_mem + LINE_HIGH + 25);
  hs_pos = LINE_HIGH + 12; /* FIXME #define */
#else
  draw_text("HIGH SCORE: ------ ---", scr_mem + LINE_HIGH + 9);
  hs_pos = LINE_HIGH + 21; /* FIXME #define */
#endif

  draw_number(high_score, 6, scr_mem + hs_pos);
  draw_text(high_score_name, scr_mem + hs_pos + 7);

  /* Version & Date: */
  draw_text("VERSION: ", scr_mem + LINE_VERSION);
  draw_text(VERSION, scr_mem + LINE_VERSION + 9);
  draw_text(DATE, scr_mem + LINE_VERSION + 40 - strlen(DATE));


  /* Activate the screen */
  OS.sdmctl = (DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_FETCH);

  /* Init sound effects */
  siren_ctr1 = 0;
  siren_ctr2 = 0;
  siren_pitch = 100;
  siren_doppler = 23;
  siren_doppler_dir = 1;
  honk = 0;

  POKEY_WRITE.audctl = 0;
  POKEY_WRITE.skctl = 3;


  /* Clear title screen loop input state */
  select_down = 0;
  select_down_wait = 0;
  option_down = 0;
  levels_by_wait = 0;
  levels_by_idx = 0;
  cmd = CMD_PLAY;

  /* (Eat any input) */
  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);
  OS.ch = KEY_NONE;

  /* Title screen main loop: */
  do {
    OS.color0 = OS.rtclok[2];

    /* Handle siren sound effect */
    siren_ctr1++;
    if (siren_ctr1 == 0) {
      siren_ctr2 = (siren_ctr2 + 1) & 0x0F;
      if (siren_ctr2 == 0) {
        siren_pitch = 210 - siren_pitch;

        if (siren_doppler_dir == 0) {
          siren_doppler++;
          if (siren_doppler == 0x1F) {
            siren_doppler_dir = 1;
          }
        } else {
          siren_doppler--;
          if (siren_doppler == 0x00) {
            siren_doppler_dir = 0;
          }
        }
      }
    }

    if (siren_doppler <= 15) {
      POKEY_WRITE.audf1 = siren_pitch + (siren_doppler >> 2);
      POKEY_WRITE.audc1 = 0xAF - siren_doppler;

      if (honk != 0) {
        POKEY_WRITE.audf2 = 10;
        POKEY_WRITE.audc2 = 0xCF - siren_doppler;
        POKEY_WRITE.audf3 = 30;
        POKEY_WRITE.audc3 = 0xCF - siren_doppler;
        POKEY_WRITE.audf4 = 50;
        POKEY_WRITE.audc4 = 0xCF - siren_doppler;

        if (siren_ctr1 == 0x00 || siren_ctr1 == 0x08)
          honk--;
      } else {
        POKEY_WRITE.audc2 = 0;
        POKEY_WRITE.audc3 = 0;
        POKEY_WRITE.audc4 = 0;

        if (OS.stick0 != 15 || OS.stick1 != 15) {
          OS.atract = 0;

          honk = (POKEY_READ.random % 0x0F) + 15;
        }
      }
    }

    /* Select: Change starting level */
    if (CONSOL_SELECT(GTIA_READ.consol) == 1) {
      OS.atract = 0;

      if (select_down == 0) {
        /* Initial press of Select */
        if (level < levels_data[0]) {
          level++;
        } else {
          level = 1;
          select_down_wait = 0;
        }

        draw_number(level, 2, scr_mem + level_pos);
        select_down = 64;
      } else {
        /* Holding Select down: */
        if (select_down_wait < 1536) {
          /* Wait before initial repeat */
          select_down_wait++;
        } else if (select_down_wait < 16384) {
          /* Slow repeat at first */
          select_down_wait++;
          select_down += 64;
        } else {
          /* Fast repeat */
          if (select_down < 65536 - 512)
            select_down += 512;
          else
            select_down = 0;
        }
      }
    } else {
      /* Not pressing/holding Select */
      select_down = 0;
      select_down_wait = 0;
    }

    /* Option: Toggle controls */
    if (CONSOL_OPTION(GTIA_READ.consol) == 1) {
      OS.atract = 0;

      if (option_down == 0) {
        main_stick = !main_stick; /* (choices are 0 & 1) */
        show_controls();
        option_down = 1;
      }
    } else {
      option_down = 0;
    }

#ifdef DISK
    /* ? or Help: Show help screen */
    if (OS.ch == KEY_QUESTIONMARK || POKEY_READ.kbcode == KEY_HELP) {
      OS.atract = 0;

      cmd = CMD_HELP;
    }

    /* H: Show high score table */
    if (OS.ch == KEY_H) {
      OS.atract = 0;

      cmd = CMD_HIGHSCORES;
    }
#endif

    /* Handle rotating level credits line */
    levels_by_wait++;
    if (levels_by_wait > 10240) {
      levels_by_wait = 0;
      levels_by_idx++;
      if (levels_by_idx >= sizeof(additional_levels) / sizeof(char *)) {
        levels_by_idx = 0;
      }
      bzero(scr_mem + LINE_BY_2, 20);
      draw_text(additional_levels[levels_by_idx], scr_mem + LINE_BY_2 + 10 - (strlen(additional_levels[levels_by_idx]) >> 1));
    }
  } while (OS.strig0 == 1 && OS.strig1 == 1 && CONSOL_START(GTIA_READ.consol) == 0 && cmd == CMD_PLAY);

#if defined(DISK) || defined(FUJINET)
  /* Save current config (disk & fujinet), and high score (fujinet) */
  save_config();
#endif


  /* FIXME: quiet() */
  POKEY_WRITE.audc1 = 0;
  POKEY_WRITE.audc2 = 0;
  POKEY_WRITE.audc3 = 0;
  POKEY_WRITE.audc4 = 0;
  POKEY_WRITE.audf1 = 0;
  POKEY_WRITE.audf2 = 0;
  POKEY_WRITE.audf3 = 0;
  POKEY_WRITE.audf4 = 0;

  /* Disable DLI & VBI */
  OS.sdmctl = 0;
  ANTIC.nmien = NMIEN_VBI;

  OS.critic = 1;
  OS.vvblkd = OLDVEC;
  OS.critic = 0;

  /* (Eat any input) */
  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);
  OS.ch = KEY_NONE;

  return cmd;
}

/* Show controls (based on the setting choice) */
void show_controls(void) {
  if (main_stick == STICK_LEFT) {
    draw_text("USE LEFT JOYSTICK TO MOVE. ", scr_mem + LINE_CONTROLS + 7);
    draw_text("USE RIGHT STICK (OR LEFT STICK + FIRE)", scr_mem + LINE_CONTROLS + 40 + 1);
  } else {
    draw_text("USE RIGHT JOYSTICK TO MOVE.", scr_mem + LINE_CONTROLS + 7);
    draw_text("USE LEFT STICK (OR RIGHT STICK + FIRE)", scr_mem + LINE_CONTROLS + 40 + 1);
  }
}

