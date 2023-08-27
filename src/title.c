/*
  Firefighter title screen

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2023-08-27
*/

#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include "title.h"
#include "shapes.h"
#include "draw_text.h"
#include "dli.h"
#include "game.h"

#ifdef DISK
#include "config.h"
#endif

extern unsigned char font1_data[];
extern unsigned char scr_mem[];
extern unsigned char levels_data[];
extern unsigned char * dlist;

extern char level;
extern char main_stick;
extern unsigned long int high_score;
extern char high_score_name[4];

void show_controls(void);

/* Set up and display title screen; title screen loop */
char show_title(void) {
  int i;
  unsigned char siren_ctr1, siren_ctr2, siren_pitch, siren_doppler, siren_doppler_dir, honk;
  unsigned int select_down, level_pos, select_down_wait;
  unsigned char option_down, want_help;

  /* FIXME: Screen setup could be moved to a function -bjk 2023.08.22 */

  OS.sdmctl = 0;

  bzero(scr_mem, 1024);

  /* Generate a display list... */
  POKE(dlist, DL_BLK8);
  POKE(dlist + 1, DL_BLK8);
  POKE(dlist + 2, DL_BLK1);

  POKE(dlist + 3, DL_LMS(DL_GRAPHICS0));
  POKEW(dlist + 4, (unsigned int) scr_mem);
  POKE(dlist + 6, DL_BLK2);

  POKE(dlist + 7, DL_GRAPHICS2);
  POKE(dlist + 8, DL_BLK2);
  POKE(dlist + 9, DL_GRAPHICS1);

  POKE(dlist + 10, DL_DLI(DL_BLK8));

  for (i = 0; i < 3; i++) {
    POKE(dlist + 11 + i * 4, DL_GRAPHICS0);
    POKE(dlist + 11 + i * 4 + 1, DL_BLK2);
    POKE(dlist + 11 + i * 4 + 2, DL_GRAPHICS2);
    POKE(dlist + 11 + i * 4 + 3, DL_BLK6);
  }

  POKE(dlist + 26, DL_BLK2);
  for (i = 27; i < 30; i++)
    POKE(dlist + i, DL_GRAPHICS0);
  POKE(dlist + 30, DL_BLK1);

  POKE(dlist + 31, DL_GRAPHICS0);
  POKE(dlist + 32, DL_GRAPHICS0);

  POKE(dlist + 33, DL_BLK4);
  POKE(dlist + 34, DL_GRAPHICS0);

  POKE(dlist + 35, DL_BLK1);
  POKE(dlist + 36, DL_GRAPHICS0);

  POKE(dlist + 37, DL_JVB);
  POKEW(dlist + 38, (unsigned int) dlist);

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

  /* Enable DLI rotuine */
  ANTIC.nmien = NMIEN_VBI;
  while (ANTIC.vcount < 124);
  OS.vdslst = (void *) dli;
  ANTIC.nmien = NMIEN_VBI | NMIEN_DLI;

  /* Title & Credits */
  draw_text("NEW BREED SOFTWARE PRESENTS:", scr_mem + 6);
  draw_text("@ FIREFIGHTER! @", scr_mem + 40 + 2);
  draw_text("bill kendrick", scr_mem + 60 + 1);
  /* "2023" */
  POKE(scr_mem + 75, 2 + 16 + 64);
  POKE(scr_mem + 76, 0 + 16 + 64);
  POKE(scr_mem + 77, 2 + 16 + 64);
  POKE(scr_mem + 78, 3 + 16 + 64);

  /* Help section 1 */
  draw_text("LEAD TO EXIT. PUT OUT FIRE. WALL. PIPES.", scr_mem + 80 + 0);

  POKE(scr_mem + (80 + 40) + 0, CIVILIAN);
  POKE(scr_mem + (80 + 40) + 1, CIVILIAN);
  POKE(scr_mem + (80 + 40) + 2, FIREFIGHTER_RIGHT);

  POKE(scr_mem + (80 + 40) + 3, EXIT1);
  POKE(scr_mem + (80 + 40) + 4, EXIT2);
  POKE(scr_mem + (80 + 40) + 5, DOOR);

  POKE(scr_mem + (80 + 40) + 7, FIRE_LG);
  POKE(scr_mem + (80 + 40) + 8, FIRE_MD);
  POKE(scr_mem + (80 + 40) + 9, FIRE_SM);

  POKE(scr_mem + (80 + 40) + 10, 'e' + 64);
  POKE(scr_mem + (80 + 40) + 11, 'g' + 64);
  POKE(scr_mem + (80 + 40) + 12, FIREFIGHTER_LEFT);

  POKE(scr_mem + (80 + 40) + 14, WALL);
  POKE(scr_mem + (80 + 40) + 15, WALL);

  POKE(scr_mem + (80 + 40) + 17, PIPE_DOWN_RIGHT);
  POKE(scr_mem + (80 + 40) + 18, PIPE_LEFT_RIGHT);
  POKE(scr_mem + (80 + 40) + 19, PIPE_UP_LEFT);

  /* Help section 2 */
  draw_text("PUSH CRATES&OIL. AX BREAKS CRATES&PIPES", scr_mem + 140 + 0);

  POKE(scr_mem + (140 + 40) + 2, FIREFIGHTER_RIGHT);
  POKE(scr_mem + (140 + 40) + 3, CRATE);
  POKE(scr_mem + (140 + 40) + 6, OIL);

  POKE(scr_mem + (140 + 40) + 9, AX);

  POKE(scr_mem + (140 + 40) + 14, CRATE_BROKEN);

  POKE(scr_mem + (140 + 40) + 17, GASLEAK_LEFT);
  POKE(scr_mem + (140 + 40) + 18, PIPE_BROKEN_UP_DOWN);
  POKE(scr_mem + (140 + 40) + 19, GASLEAK_RIGHT);


  /* Help section 3 */
  draw_text("TURN VALVE. OIL&LEAKS EXPLODE! PUSH EXIT", scr_mem + 200 + 0);

  POKE(scr_mem + (200 + 40) + 1, PIPE_LEFT_RIGHT);
  POKE(scr_mem + (200 + 40) + 2, VALVE_OPEN);
  POKE(scr_mem + (200 + 40) + 3, PIPE_LEFT_RIGHT);

  POKE(scr_mem + (200 + 40) + 7, FIRE_LG);
  POKE(scr_mem + (200 + 40) + 8, FIRE_LG);
  POKE(scr_mem + (200 + 40) + 9, OIL);
  POKE(scr_mem + (200 + 40) + 10, FIRE_LG);

  POKE(scr_mem + (200 + 40) + 16, FIREFIGHTER_RIGHT);
  POKE(scr_mem + (200 + 40) + 17, DOOR);

  /* Control instructions */
  show_controls();
  draw_text("TO SPRAY.", scr_mem + 340 + 16);

  /* Title screen control instructions */
  draw_text("START/FIRE: BEGIN - OPTION: SWAP STICKS", scr_mem + 380 + 0);
#ifdef DISK
  draw_text("SELECT: STARTING LEVEL -- - HELP/?: INFO", scr_mem + 420);
  level_pos = 443;
#else
  draw_text("SELECT: STARTING LEVEL --", scr_mem + 420 + 7);
  level_pos = 450;
#endif
  draw_number(level, 2, scr_mem + level_pos);

#if 0
  /* High score */
  draw_text("HIGH SCORE: ------ ---", scr_mem + 460 + 9);

  draw_number(high_score, 6, scr_mem + 481);
  draw_text(high_score_name, scr_mem + 488);
#endif

  /* Version & Date: */
  draw_text("VERSION: ", scr_mem + 500);
  draw_text(VERSION, scr_mem + 500 + 9);
  draw_text(DATE, scr_mem + 500 + 40 - strlen(DATE));

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
  want_help = 0;

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
        if (level < levels_data[0])
          level++;
        else
          level = 1;

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

//    while (ANTIC.vcount < 124);

#ifdef DISK
    /* ? or Help: Show help screen */
    if (OS.ch == KEY_QUESTIONMARK || POKEY_READ.kbcode == KEY_HELP) {
      OS.atract = 0;

      want_help = 1;
    }
#endif
  } while (OS.strig0 == 1 && OS.strig1 == 1 && CONSOL_START(GTIA_READ.consol) == 0 && !want_help);

#ifdef DISK
  /* Save current config */
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

  /* Disable DLI */
  OS.sdmctl = 0;
  ANTIC.nmien = NMIEN_VBI;

  /* (Eat any input) */
  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);
  OS.ch = KEY_NONE;

  return want_help;
}

/* Show controls (based on the setting choice) */
void show_controls(void) {
  if (main_stick == STICK_LEFT) {
    draw_text("USE LEFT JOYSTICK TO MOVE. ", scr_mem + 260 + 7);
    draw_text("USE RIGHT STICK (OR LEFT STICK + FIRE)", scr_mem + 300 + 1);
  } else {
    draw_text("USE RIGHT JOYSTICK TO MOVE.", scr_mem + 260 + 7);
    draw_text("USE LEFT STICK (OR RIGHT STICK + FIRE)", scr_mem + 300 + 1);
  }
}

