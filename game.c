/*
  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/

  2023-08-15 - 2023-08-15
*/

#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include "game.h"
#include "shapes.h"
#include "draw_text.h"
#include "dli.h"

extern unsigned char font1_data[];
// extern unsigned char font2_data[]; /* Not actually referenced */

extern unsigned char scr_mem[];
extern unsigned char * dlist;

void setup_game_screen(void);
#define shape_at(x, y) (PEEK(scr_mem + 60 + ((y) * 20) + (x)))
#define set_shape(x, y, s) POKE(scr_mem + 60 + ((y) * 20) + (x), (s))
#define obstacle(s) ((s) != 0 && (s) != AX)
void draw_level(void);
void cellular_automata(void);
unsigned char flammable(unsigned char c);
unsigned char valid_dir(unsigned char x, unsigned char y, unsigned char dir);
void explode(char x, char y);
unsigned char spray(unsigned char x, unsigned char y, unsigned char want_shape);

#define DIR_LEFT 0
#define DIR_RIGHT 1

char exploding;

void start_game(void) {
  unsigned char ply_x, ply_y, ply_dir, want_x, want_y, push_x, push_y, stick, shape, shape2, have_ax;

  setup_game_screen();
  draw_level();

  ply_x = 0;
  ply_y = 9;
  ply_dir = DIR_RIGHT;
  have_ax = 0;
  exploding = 0;

  do {
    /* Control the player based on Joystick 1 input */

    want_x = ply_x;
    want_y = ply_y;
    push_x = ply_x;
    push_y = ply_y;

    stick = (OS.stick0 ^ 0x0F);

#define STK_BIT_RIGHT 8
#define STK_BIT_LEFT 4
#define STK_BIT_DOWN 2
#define STK_BIT_UP 1

    if (stick & STK_BIT_RIGHT) {
      ply_dir = 1;
      if (ply_x < 19) {
        want_x++;
        push_x += 2;
      }
    }
    if (stick & STK_BIT_LEFT) {
      ply_dir = 0;
      if (ply_x > 0) {
        want_x--;
        push_x -= 2;
      }
    }
    if (stick & STK_BIT_DOWN) {
      if (ply_y < 10) {
        want_y++;
        push_y += 2;
      }
    }
    if (stick & STK_BIT_UP) {
      if (ply_y > 0) {
        want_y--;
        push_y -= 2;
      }
    }

    if (want_x != ply_x || want_y != ply_y) {
      shape = shape_at(want_x, want_y);

      /* Want to move to a new position */
      if (!obstacle(shape) || (shape >= 32 + 128 && shape < 64 + 128) /* Water */) {
        /* We can! Move there! */
        set_shape(ply_x, ply_y, 0);
        ply_x = want_x;
        ply_y = want_y;

        if (shape == AX) {
          have_ax = 1;
          draw_text("ax", scr_mem + 40 + 18);
        }
      } else if (shape == CRATE || shape == CRATE_BROKEN || shape == OIL) {
        /* Hitting a crate or oil barrel */
        if (have_ax && shape != OIL) {
          /* Have the ax? You can break crates! */
          set_shape(want_x, want_y, (shape == CRATE ? CRATE_BROKEN : 0));
        } else {
          /* No ax, or note a crate, see whether we can push it */
          shape2 = shape_at(push_x, push_y);
          if (shape2 == 0) {
            set_shape(want_x, want_y, 0);
            set_shape(push_x, push_y, shape);
            set_shape(ply_x, ply_y, 0);
            ply_x = want_x;
            ply_y = want_y;
          }
        }
      }
    }

    /* Draw the player */
    set_shape(ply_x, ply_y, FIREMAN_LEFT + ply_dir);


    /* Cellular automata the screen */
    cellular_automata();


    /* Spray water based on Joystick 2 input */

#define WATER_N  (35 + 128)
#define WATER_NW (60 + 128) 
#define WATER_W  (39 + 128)
#define WATER_SW (52 + 128) 
#define WATER_S  (40 + 128)
#define WATER_SE (48 + 128) 
#define WATER_E  (44 + 128)
#define WATER_NE (56 + 128) 

    stick = OS.stick1;
    if (stick == 7) {
      /* Right (East) */
      if (spray(ply_x + 1, ply_y,   WATER_E)) {
        spray(ply_x + 2, ply_y - 1, WATER_E + 1);
        spray(ply_x + 2, ply_y,     WATER_E + 2);
        spray(ply_x + 2, ply_y + 1, WATER_E + 3);
      }
    } else if (stick == 11) {
      /* Left (West) */
      if (spray(ply_x - 1, ply_y,     WATER_W)) {
        spray(ply_x - 2, ply_y - 1, WATER_W - 3);
        spray(ply_x - 2, ply_y,     WATER_W - 2);
        spray(ply_x - 2, ply_y + 1, WATER_W - 1);
      }
    } else if (stick == 13) {
      /* Down (South) */
      if (spray(ply_x    , ply_y + 1, WATER_S)) {
        spray(ply_x - 1, ply_y + 2, WATER_S + 1);
        spray(ply_x    , ply_y + 2, WATER_S + 2);
        spray(ply_x + 1, ply_y + 2, WATER_S + 3);
      }
    } else if (stick == 14) {
      /* Up (North) */
      if (spray(ply_x    , ply_y - 1, WATER_N)) {
        spray(ply_x - 1, ply_y - 2, WATER_N - 3);
        spray(ply_x    , ply_y - 2, WATER_N - 2);
        spray(ply_x + 1, ply_y - 2, WATER_N - 1);
      }
    } else if (stick == 5) {
      /* Down + Right (Southeast) */
      if (spray(ply_x + 1, ply_y + 1, WATER_SE)) {
        spray(ply_x + 2, ply_y + 1, WATER_SE + 1);
        spray(ply_x + 1, ply_y + 2, WATER_SE + 2);
        spray(ply_x + 2, ply_y + 2, WATER_SE + 3);
      }
    } else if (stick == 9) {
      /* Down + Left (Southwest) */
      if (spray(ply_x - 1, ply_y + 1, WATER_SW + 1)) {
        spray(ply_x - 2, ply_y + 1, WATER_SW);
        spray(ply_x - 2, ply_y + 2, WATER_SW + 2);
        spray(ply_x - 1, ply_y + 2, WATER_SW + 3);
      }
    } else if (stick == 6) {
      /* Up + Right (Northeast) */
      if (spray(ply_x + 1, ply_y - 1, WATER_NE + 2)) {
        spray(ply_x + 1, ply_y - 2, WATER_NE);
        spray(ply_x + 2, ply_y - 2, WATER_NE + 1);
        spray(ply_x + 2, ply_y - 1, WATER_NE + 3);
      }
    } else if (stick == 10) {
      /* Up + Left (Northwest) */
      if (spray(ply_x - 1, ply_y - 1, WATER_NW + 3)) {
        spray(ply_x - 2, ply_y - 2, WATER_NW);
        spray(ply_x - 1, ply_y - 2, WATER_NW + 1);
        spray(ply_x - 2, ply_y - 1, WATER_NW + 2);
      }
    }

    if (exploding) {
      OS.color4 = exploding;
      exploding--;
      if (exploding == 0)
        POKE(dlist + 6, DL_BLK1);
      else
        POKE(dlist + 6, 16 + (16 << (POKEY_READ.random % 3)));
      POKEY_WRITE.audf1 = POKEY_READ.random;
      POKEY_WRITE.audc1 = exploding;
    }

    while (ANTIC.vcount < 124);
  } while (OS.strig0 == 1 && OS.strig1 == 1 && CONSOL_START(GTIA_READ.consol) == 0);

  OS.sdmctl = 0;
  ANTIC.nmien = NMIEN_VBI;

  do {
  } while (CONSOL_START(GTIA_READ.consol) == 1);
}

unsigned char spray(unsigned char x, unsigned char y, unsigned char want_shape) {
  unsigned char shape;

  shape = shape_at(x, y);
  if (shape == FIRE_LG) {
    set_shape(x, y, FIRE_MD);
    return 0;
  } else if (shape == FIRE_MD) {
    set_shape(x, y, FIRE_SM);
    return 0;
  } else if (shape == FIRE_SM) {
    set_shape(x, y, 0);
    return 0;
  } else if (obstacle(shape)) {
    return 0;
  }

  set_shape(x, y, want_shape);
  return 1;
}

void setup_game_screen(void) {
  int i;

  OS.sdmctl = 0;

  bzero(scr_mem, 1024);

  /* Generate a display list... */
  POKE(dlist, DL_BLK8);
  POKE(dlist + 1, DL_BLK8);
  POKE(dlist + 2, DL_BLK4);

  POKE(dlist + 3, DL_LMS(DL_GRAPHICS0));
  POKEW(dlist + 4, (unsigned int) scr_mem);
  POKE(dlist + 6, DL_BLK1);

  POKE(dlist + 7, DL_GRAPHICS2);

  POKE(dlist + 8, DL_DLI(DL_BLK1));

  for (i = 9; i < 20; i++) {
    POKE(dlist + i, DL_GRAPHICS2);
  }

  POKE(dlist + 21, DL_JVB);
  POKEW(dlist + 22, (unsigned int) dlist);

  OS.sdlst = dlist;
  OS.chbas = (unsigned char) ((unsigned int) font1_data / 256);

  POKE(0x600, OS.chbas + 2);

  OS.color0 = 0x52;

  ANTIC.nmien = NMIEN_VBI;
  while (ANTIC.vcount < 124);
  OS.vdslst = (void *) dli;
  ANTIC.nmien = NMIEN_VBI | NMIEN_DLI;

  draw_text("LEVEL: 00  SCORE: 000000", scr_mem + 0);
  draw_text("@ FIREFIGHTER! @", scr_mem + 40 + 2);

  OS.sdmctl = 34;
}

void draw_level(void) {
  int i, x, y;

  for (i = 0; i < 20; i++) {
    set_shape(i, 0, WALL);
    set_shape(i, 10, WALL);
  }

  for (i = 0; i < 10; i++) {
    set_shape(0, i, WALL);
    set_shape(19, i, WALL);
  }

  for (i = 0; i < 10; i++) {
    do {
      x = POKEY_READ.random % 20;
      y = POKEY_READ.random % 10;
    } while (PEEK(scr_mem + 60 + y * 20 + x) != 0);

    set_shape(x, y, CRATE + (POKEY_READ.random % 2));
  }

  set_shape(18, 1, FIRE_SM);
  set_shape(9, 4, AX);
  set_shape(16, 2, OIL);
  set_shape(15, 2, OIL);
  set_shape(16, 3, OIL);
  set_shape(15, 3, OIL);
  set_shape(18, 5, OIL);
}

int dir_x[8] = {  0,  1, 1, 1, 0, -1, -1, -1 };
int dir_y[8] = { -1, -1, 0, 1, 1,  1,  0, -1 };

void cellular_automata(void) {
  char x, y;
  unsigned shape, shape2, ignited_shape, dir, rand;

  for (y = 0; y < 10; y++) {
    for (x = 0; x < 20; x++) {
      shape = shape_at(x, y);
      rand = POKEY_READ.random;

      if (shape == FIRE_SM && rand < 16) {
        /* Grow small fire to medium */
        set_shape(x, y, FIRE_MD);
      } else if (shape == FIRE_MD && rand < 16) {
        /* Grow medium fire to large */
        set_shape(x, y, FIRE_LG);
      } else if (shape == FIRE_LG && rand < 32) {
        /* Large fire tries to spread */
        dir = POKEY_READ.random % 9;
        if (valid_dir(x, y, dir)) {
          shape2 = shape_at(x + dir_x[dir], y + dir_y[dir]);
          ignited_shape = flammable(shape2);
          if (ignited_shape == FIRE_XLG) {
            explode(x + dir_x[dir], y + dir_y[dir]);
          } else if (ignited_shape != 0) {
            set_shape(x + dir_x[dir], y + dir_y[dir], ignited_shape);
          }
        }
      } else if (shape >= 32 + 128 && shape < 64 + 128) {
        /* Erase water */
        set_shape(x, y, 0);
      }
    }
  }
}

void explode(char x, char y) {
  char shape, flam;

  set_shape(x, y, FIRE_LG);

  if (x > 0) {
    shape = shape_at(x - 1, y);
    flam = flammable(shape);
    if (flam && flam != FIRE_XLG)
      set_shape(x - 1, y, FIRE_LG);
  }

  if (x < 19) {
    shape = shape_at(x + 1, y);
    flam = flammable(shape);
    if (flam && flam != FIRE_XLG)
      set_shape(x + 1, y, FIRE_LG);
  }

  if (y > 0) {
    shape = shape_at(x, y - 1);
    flam = flammable(shape);
    if (flam && flam != FIRE_XLG)
      set_shape(x, y - 1, FIRE_LG);
  }

  if (y < 10) {
    shape = shape_at(x, y + 1);
    flam = flammable(shape);
    if (flam && flam != FIRE_XLG)
      set_shape(x, y + 1, FIRE_LG);
  }

  exploding = 15;
}

unsigned char valid_dir(unsigned char x, unsigned char y, unsigned char dir) {
  int dx, dy;

  dx = dir_x[dir];
  dy = dir_y[dir];
  return !((dx == -1 && x == 0) ||
      (dy == -1 && y == 0) ||
      (dx == 1 && x == 19) ||
      (dy == 1 && y == 10));
};

unsigned char flammable(unsigned char c) {
  if (c == OIL || c == GASLEAK_RIGHT || c == GASLEAK_LEFT || c == GASLEAK_UP || c == GASLEAK_DOWN) {
    /* Oil barrel and gas leaks cause an explosion */
    return FIRE_XLG;
  } else if (c == CRATE || c == CRATE_BROKEN || c == AX) {
    /* Crates and ax ignite fully */
    return FIRE_LG;
  } else if (c == 0) {
    /* Empty spaces ignite smally */
    return FIRE_SM;
  } else {
    /* Other things do not ignite */
    return 0;
  }
}

