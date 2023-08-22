/*
  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2023-08-22
*/

#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include "title.h"
#include "shapes.h"
#include "draw_text.h"
#include "dli.h"
#include "game.h"

extern unsigned char font1_data[];
// extern unsigned char font2_data[]; /* Not actually referenced */

extern unsigned char scr_mem[];
extern unsigned char levels_data[];
extern unsigned char * dlist;

extern char level;
extern unsigned long int high_score;
extern char high_score_name[4];
extern char main_stick;

void show_controls(void);

/* FIXME */
char show_title(void) {
  int i;
  unsigned char siren_ctr1, siren_ctr2, siren_pitch, siren_doppler, siren_doppler_dir, honk;
  unsigned int select_down, level_pos;
  unsigned char option_down, want_help;

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
  OS.chbas = (unsigned char) ((unsigned int) font1_data / 256);

  POKE(0x600, OS.chbas + 2);

  OS.color0 = 0x52;
  OS.color1 = 0xCA;
  OS.color2 = 0x02;
  OS.color3 = 0x46;
  OS.color4 = 0x02;

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
  draw_text("GAS PIPE  LEAKING GAS  TURN VALVE", scr_mem + 80 + 3);
  POKE(scr_mem + 122, PIPE_DOWN_RIGHT);
  POKE(scr_mem + 123, PIPE_LEFT_RIGHT);
  POKE(scr_mem + 124, PIPE_UP_LEFT);

  POKE(scr_mem + 128, PIPE_BROKEN_UP_DOWN);
  POKE(scr_mem + 129, GASLEAK_RIGHT);

  POKE(scr_mem + 134, PIPE_LEFT_RIGHT);
  POKE(scr_mem + 135, VALVE_OPEN);
  POKE(scr_mem + 136, PIPE_LEFT_RIGHT);

  /* Help section 2 */
  draw_text("GET AX  BREAK CRATES   OIL    WALLS", scr_mem + 140 + 2);
  POKE(scr_mem + 182, AX);

  POKE(scr_mem + 187, CRATE);
  POKE(scr_mem + 188, CRATE_BROKEN);

  POKE(scr_mem + 193, OIL);

  POKE(scr_mem + 196, WALL);
  POKE(scr_mem + 197, WALL);
  POKE(scr_mem + 198, WALL);

  /* Help section 3 */
  draw_text("PUT OUT FIRE, LEAD CIVILIANS TO EXIT", scr_mem + 200 + 2);
  POKE(scr_mem + 240, FIRE_LG);
  POKE(scr_mem + 241, FIRE_MD);
  POKE(scr_mem + 242, FIRE_SM);

  POKE(scr_mem + 243, 'e' + 64);
  POKE(scr_mem + 244, 'g' + 64);
  POKE(scr_mem + 245, FIREFIGHTER_LEFT);

  POKE(scr_mem + 248, CIVILIAN);
  POKE(scr_mem + 249, CIVILIAN);
  POKE(scr_mem + 250, FIREFIGHTER_RIGHT);

  POKE(scr_mem + 255, DOOR);
  POKE(scr_mem + 257, EXIT1);
  POKE(scr_mem + 258, EXIT2);

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


  select_down = 0;
  option_down = 0;
  want_help = 0;

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
  
        if (OS.stick0 != 15 || OS.stick1 != 15)
          honk = (POKEY_READ.random % 0x0F) + 15;
      }
    }

    /* Select: Change starting level */
    if (CONSOL_SELECT(GTIA_READ.consol) == 1) {
      if (select_down == 0) {
        if (level < levels_data[0])
          level++;
        else
          level = 1;

        draw_number(level, 2, scr_mem + level_pos);
        select_down = 32;
      } else {
        select_down += 32;
      }
    } else {
      select_down = 0;
    }

    /* Option: Toggle controls */
    if (CONSOL_OPTION(GTIA_READ.consol) == 1) {
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
      want_help = 1;
    }
#endif
  } while (OS.strig0 == 1 && OS.strig1 == 1 && CONSOL_START(GTIA_READ.consol) == 0 && !want_help);

  OS.ch = KEY_NONE;

  /* FIXME: quiet() */
  POKEY_WRITE.audc1 = 0;
  POKEY_WRITE.audc2 = 0;
  POKEY_WRITE.audc3 = 0;
  POKEY_WRITE.audc4 = 0;
  POKEY_WRITE.audf1 = 0;
  POKEY_WRITE.audf2 = 0;
  POKEY_WRITE.audf3 = 0;
  POKEY_WRITE.audf4 = 0;

  OS.sdmctl = 0;
  ANTIC.nmien = NMIEN_VBI;

  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);

  return want_help;
}

/* FIXME */
void show_controls(void) {
  if (main_stick == STICK_LEFT) {
    draw_text("USE LEFT JOYSTICK TO MOVE. ", scr_mem + 260 + 7);
    draw_text("USE RIGHT STICK (OR LEFT STICK + FIRE)", scr_mem + 300 + 1);
  } else {
    draw_text("USE RIGHT JOYSTICK TO MOVE.", scr_mem + 260 + 7);
    draw_text("USE LEFT STICK (OR RIGHT STICK + FIRE)", scr_mem + 300 + 1);
  }
}

