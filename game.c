/*
  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/

  2023-08-15 - 2023-08-17
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

int dir_x[8] = {  0,  1, 1, 1, 0, -1, -1, -1 };
int dir_y[8] = { -1, -1, 0, 1, 1,  1,  0, -1 };

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
void leak_gas(char x, char y, char dir, char on_off);
void follow_pipes(char x, char y, char on_off);
void set_sound(char p, char pch, char dist, char vol, char volch);

#define DIR_LEFT 0
#define DIR_RIGHT 1

/* FIXME */
char exploding;
char hit_pitch, hit_pitch_change, hit_volume, hit_distortion, hit_vol_decrease;
char odd_even;
unsigned char ply_x, ply_y;


/* The main game loop! */
void start_game(void) {
  unsigned char ply_dir, want_x, want_y, push_x, push_y, stick, shape, shape2, have_ax;

  setup_game_screen();
  draw_level();

  ply_x = 1;
  ply_y = 9;
  ply_dir = DIR_RIGHT;
  have_ax = 0;
  exploding = 0;
  odd_even = 0;

  set_sound(0, 0, 0, 0, 0);

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

    if (OS.strig0) {
      /* (Left stick, without trigger, to move) */
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
          draw_text("ax", scr_mem + 0 + 18);
          set_sound(20, 0, 0xA0, 14, 2);
        }
      } else if (shape == CRATE || shape == CRATE_BROKEN || shape == OIL) {
        /* Hitting a crate or oil barrel */

        if (have_ax && shape != OIL) {
          /* Have the ax? You can break crates! */

          set_shape(want_x, want_y, (shape == CRATE ? CRATE_BROKEN : 0));

          set_sound(200, 0, 0x60, 10, 5);
        } else {
          /* No ax, or not a crate? See whether we can push it */

          shape2 = shape_at(push_x, push_y);
          if (shape2 == 0) {
            set_shape(want_x, want_y, 0);
            set_shape(push_x, push_y, shape);
            set_shape(ply_x, ply_y, 0);
            ply_x = want_x;
            ply_y = want_y;

            set_sound(200, 0, 0xA0, 10, 10);
          }
        }
      } else if (shape == VALVE_OPEN) {
        /* Close an opened valve (and disable leaks) */

        set_shape(want_x, want_y, VALVE_CLOSED);
        follow_pipes(want_x, want_y, 0);

        set_sound(200, 0, 0xA0, 8, 4);
      } else if (shape == VALVE_CLOSED) {
        /* Open a closed valve */

        set_shape(want_x, want_y, VALVE_OPEN);
        /* (Cellular automata will cause leaks) */

        set_sound(100, 0, 0xA0, 8, 4);
      } else if ((shape == PIPE_UP_DOWN || shape == PIPE_LEFT_RIGHT) && have_ax) {
        /* Break a pipe with the ax (downside of having it!) */
        set_shape(want_x, want_y, shape + 1);

        set_sound(2, 0, 0xA0, 6, 3);
      } else if (shape == DOOR || shape == EXIT1 || shape == EXIT2) {
        char dir, x, y;

        for (dir = 0; dir < 8; dir++) {
          x = ply_x + dir_x[dir];
          y = ply_y + dir_y[dir];
          if (valid_dir(x, y, dir)) {
            if (shape_at(x, y) == CIVILIAN) {
              set_shape(x, y, 0);
            }
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

    /* (Alternatively, left stick + trigger to spray) */
    if (OS.strig0)
      stick = OS.stick1;
    else
      stick = OS.stick0;

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

    /* Play water spray sound effect */
    if (stick != 15) {
      POKEY_WRITE.audf3 = 0;
      POKEY_WRITE.audc3 = 0x06;
    } else {
      POKEY_WRITE.audf3 = 0;
      POKEY_WRITE.audc3 = 0;
    }

    /* Explosion sound & visual effect */
    if (exploding) {
      POKE(0x601, exploding);
      exploding--;
      if (exploding == 0)
        POKE(dlist + 6, DL_BLK1);
      else
        POKE(dlist + 6, 16 + (16 << (POKEY_READ.random % 3)));
      POKEY_WRITE.audf4 = POKEY_READ.random;
      POKEY_WRITE.audc4 = exploding;
    }

    /* Action sound effects */
    POKEY_WRITE.audf2 = hit_pitch;
    POKEY_WRITE.audc2 = hit_distortion + hit_volume;
    if (hit_volume > 0) {
      hit_volume -= hit_vol_decrease;
      hit_pitch += hit_pitch_change;
    }

    while (ANTIC.vcount < 124);
  } while (CONSOL_START(GTIA_READ.consol) == 0);

  POKE(0x601, 0);

  POKEY_WRITE.audf1 = 0;
  POKEY_WRITE.audc1 = 0;
  POKEY_WRITE.audf2 = 0;
  POKEY_WRITE.audc2 = 0;
  POKEY_WRITE.audf3 = 0;
  POKEY_WRITE.audc3 = 0;
  POKEY_WRITE.audf4 = 0;
  POKEY_WRITE.audc4 = 0;

  OS.sdmctl = 0;
  ANTIC.nmien = NMIEN_VBI;

  do {
  } while (CONSOL_START(GTIA_READ.consol) == 1);
}

/* Draw a water spray, if possible. If touching fire,
   the fire will be diminished. Return whether or not
   we drew anything (each spray is made of a narrow
   tip shape near the player, and three other shapes
   beyond that; we'll avoid drawing those other three
   if the first part failed to draw, to avoid being
   able to spray through solid objects! */
unsigned char spray(unsigned char x, unsigned char y, unsigned char want_shape) {
  unsigned char shape;

  shape = shape_at(x, y);
  if (shape == FIRE_LG) {
    /* Turn large fire into medium fire */
    set_shape(x, y, FIRE_MD);
    return 0;
  } else if (shape == FIRE_MD) {
    /* Turn medium fire into small fire */
    set_shape(x, y, FIRE_SM);
    return 0;
  } else if (shape == FIRE_SM) {
    /* Extinguish small fire */
    set_shape(x, y, 0);
    return 0;
  } else if (shape != 0) {
    /* Hit any other kind of obstacle! */
    return 0;
  }

  /* Nothing in the way; draw the spray shape and return success */
  set_shape(x, y, want_shape);
  return 1;
}

/* Set up the display list for the game screen,
   and enable the DLI */
void setup_game_screen(void) {
  int i;

  OS.sdmctl = 0;

  bzero(scr_mem, 1024);

  /* Generate a display list... */
  POKE(dlist, DL_BLK8);
  POKE(dlist + 1, DL_BLK8);
  POKE(dlist + 2, DL_BLK4);

  POKE(dlist + 3, DL_LMS(DL_GRAPHICS2));
  POKEW(dlist + 4, (unsigned int) scr_mem);
  POKE(dlist + 6, DL_BLK1);

  POKE(dlist + 7, DL_GRAPHICS0);

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

  OS.sdmctl = 34;
}

/* FIXME */
void draw_level(void) {
  int i, x, y;

  draw_text("@ FIREFIGHTER! @", scr_mem + 0 + 2);
  draw_text("LEVEL: 00  SCORE: 000000", scr_mem + 20 + 0);

  /* FIXME! */
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

  set_shape(1, 1, PIPE_DOWN_RIGHT);
  for (i = 2; i < 5; i++) {
    set_shape(i, 1, PIPE_LEFT_RIGHT);
    set_shape(1, i, PIPE_UP_DOWN);
  }
  set_shape(1, 3, VALVE_OPEN);
  set_shape(5, 1, PIPE_BROKEN_LEFT_RIGHT);

  set_shape(18, 1, FIRE_SM);
  set_shape(9, 4, AX);
  set_shape(16, 2, OIL);
  set_shape(15, 2, OIL);
  set_shape(16, 3, OIL);
  set_shape(15, 3, OIL);
  set_shape(18, 5, OIL);

  for (i = 0; i < 3; i++) {
    do {
      x = POKEY_READ.random % 20;
      y = POKEY_READ.random % 10;
    } while (PEEK(scr_mem + 60 + y * 20 + x) != 0);

    set_shape(x, y, CIVILIAN);
  }

  set_shape(18, 0, EXIT1);
  set_shape(19, 0, EXIT2);
  set_shape(19, 1, DOOR);
}

/* This routine analyzes the entire screen and
   causes fire to grow and spread, erases water
   (possibly ahead of it being (re)drawn), and
   causes any open pipe values to generate gas
   leaks at any broken points on their pipeline.

   (The `odd_even` toggle variable is flipped
   automatically, and causes the routine to
   run on alternating tiles each frame,
   in a checkerboard fashion.)
*/
void cellular_automata(void) {
  char x, y;
  unsigned shape, shape2, ignited_shape, dir, rand, any_fire;

  odd_even = !odd_even;
  any_fire = 0;

  for (y = 0; y < 10; y++) {
    for (x = (y % 1) + odd_even; x < 20; x++) {
      shape = shape_at(x, y);
      rand = POKEY_READ.random;

      if (shape == FIRE_SM && rand < 16) {
        /* Grow small fire to medium */
        set_shape(x, y, FIRE_MD);
        any_fire++;
      } else if (shape == FIRE_MD && rand < 16) {
        /* Grow medium fire to large */
        set_shape(x, y, FIRE_LG);
        any_fire++;
      } else if (shape == FIRE_LG && rand < 32) {
        /* Large fire tries to spread */
        dir = POKEY_READ.random % 8;
        if (valid_dir(x, y, dir)) {
          shape2 = shape_at(x + dir_x[dir], y + dir_y[dir]);
          ignited_shape = flammable(shape2);
          if (ignited_shape == FIRE_XLG) {
            explode(x + dir_x[dir], y + dir_y[dir]);
          } else if (ignited_shape != FIRE_INFLAM) {
            set_shape(x + dir_x[dir], y + dir_y[dir], ignited_shape);
          }
        }
        any_fire++;
      } else if (shape >= 32 + 128 && shape < 64 + 128) {
        /* Erase water */
        set_shape(x, y, 0);
      } else if (shape == VALVE_OPEN) {
        /* Find any leaky pipes and shoot gas out */
        follow_pipes(x, y, 1);
      } else if (shape == CIVILIAN) { // && rand < 64) {
        int want_dir, dist;

        /* Civilian */

        /* If we're near the player, walk towards him */
        want_dir = -1;
        for (dist = 3; dist >= 2; dist--) {
          for (dir = 0; dir < 8; dir++) {
            if ((x + dir_x[dir] == ply_x || x + dir_x[dir] * 2 == ply_x) &&
                (y + dir_y[dir] == ply_y || y + dir_y[dir] * 2 == ply_y)) {
              want_dir = dir;
            }
          }
        }

        /* If not near player (or occasionally), walk a random direction */
        if (want_dir == -1 || rand < 2) {
          if (rand < 32)
            want_dir = POKEY_READ.random % 8;
        }

        if (want_dir != -1) {
          dir = want_dir;

          if (valid_dir(x, y, dir)) {
            shape2 = shape_at(x + dir_x[dir], y + dir_y[dir]);
            if (shape2 == 0) {
              set_shape(x, y, 0);
              set_shape(x + dir_x[dir], y + dir_y[dir], CIVILIAN);
            }
          }
        }
      }
    }
  }

  if (any_fire) {
    POKEY_WRITE.audf1 = ((POKEY_READ.random) >> 4) + 128;
    POKEY_WRITE.audc1 = (any_fire >> 4) + 1;
  } else {
    POKEY_WRITE.audc1 = 0x00;
  }
}

/* Starting at a position (it'll be a valve),
   begin traversing any attached pipes in the four
   cardinal directions */
void follow_pipes(char x, char y, char on_off) {
  char dir, shape2;

  for (dir = 0; dir < 8; dir += 2) {
    if (valid_dir(x, y, dir)) {
      shape2 = shape_at(x + dir_x[dir], y + dir_y[dir]);
      if ((shape2 == PIPE_UP_DOWN || shape2 == PIPE_BROKEN_UP_DOWN) && dir_x[dir] == 0) {
        leak_gas(x, y + dir_y[dir], dir, on_off);
      } else if ((shape2 == PIPE_LEFT_RIGHT || shape2 == PIPE_BROKEN_UP_DOWN) && dir_y[dir] == 0) {
        leak_gas(x + dir_x[dir], y, dir, on_off);
      }
    }
  }
}

/*
  Given an input direction (4 cardinal directions), return
  a 0-7 direction value (used to follow pipe corners)

    0
  3   1
    2

  7 0 1
  6   2
  5 4 3
*/

#define NA 0

char pipe_corner[16] = {
  /* Down+Right */
  2, NA, NA, 4,
  /* Down+Left */
  6, 4, NA, NA,
  /* Up+Left */
  NA, 0, 6, NA,
  /* Up+Right */
  NA, NA, 2, 0,
};

/* Starting a given direction, follow a chain of pipes
   until we can follow them no more, and create or erase
   gas leaks at any broken pipes (will depend on the
   valve state) */
void leak_gas(char x, char y, char dir, char on_off) {
  char shape, move, check, set;

  do {
    move = 0;

    shape = shape_at(x, y);

    if (shape == PIPE_BROKEN_UP_DOWN) {
      /* Broken vertical (up/down) pipe;
         try to add (or remove) gas leak on left/rght */
      check = (on_off ? 0 : GASLEAK_LEFT);
      set = (on_off ? GASLEAK_LEFT : 0);
      if (x > 0 && shape_at(x - 1, y) == check)
        set_shape(x - 1, y, set);

      check = (on_off ? 0 : GASLEAK_RIGHT);
      set = (on_off ? GASLEAK_RIGHT : 0);
      if (x < 19 && shape_at(x + 1, y) == check)
        set_shape(x + 1, y, set);

      move = 1;
    } else if (shape == PIPE_BROKEN_LEFT_RIGHT) {
      /* Broken horizontal (left/right) pipe;
         try to add (or remove) gas leak above/below */
      check = (on_off ? 0 : GASLEAK_UP);
      set = (on_off ? GASLEAK_UP : 0);
      if (y > 0 && shape_at(x, y - 1) == check)
        set_shape(x, y - 1, set);

      check = (on_off ? 0 : GASLEAK_DOWN);
      set = (on_off ? GASLEAK_DOWN : 0);
      if (y < 10 && shape_at(x, y + 1) == check)
        set_shape(x, y + 1, set);

      move = 1;
    } else if (shape == PIPE_UP_DOWN || shape == PIPE_LEFT_RIGHT) {
      /* Non-broken up/down or left/right;
         keep going the same direction */
      move = 1;
    } else if (shape == PIPE_DOWN_RIGHT || shape == PIPE_DOWN_LEFT ||
               shape == PIPE_UP_RIGHT || shape == PIPE_UP_LEFT) {
      /* Corner pipe; turn a new direction, based on
         the direction we were going when we hit it */
      dir = pipe_corner[(shape - PIPE_DOWN_RIGHT) * 4 + (dir / 2)];

      move = 1;
    }

    /* Valid pipe? Move into it. Otherwise, all done */
    if (move) {
      x = x + dir_x[dir];
      y = y + dir_y[dir];
    }
  } while (move);
}


/* Create an explosion of fire at the given position
   (occurs when fire touches oil barrels or gas leaks) */
void explode(char x, char y) {
  char shape, flam;

  /* Turn the spot into fire */
  set_shape(x, y, FIRE_LG);

  /* Try to turn each spot in the 4 cardinal directions
     into fire, too.  (Note: We avoid immediately turning
     an exlosive (FIRE_XLG response) object into fire,
     to help encourage a "chain reaction" effect!) */

  if (x > 0) {
    /* Left */
    shape = shape_at(x - 1, y);
    flam = flammable(shape);
    if (flam != FIRE_INFLAM && flam != FIRE_XLG)
      set_shape(x - 1, y, FIRE_LG);
  }

  if (x < 19) {
    /* Right */
    shape = shape_at(x + 1, y);
    flam = flammable(shape);
    if (flam != FIRE_INFLAM && flam != FIRE_XLG)
      set_shape(x + 1, y, FIRE_LG);
  }

  if (y > 0) {
    /* Up */
    shape = shape_at(x, y - 1);
    flam = flammable(shape);
    if (flam != FIRE_INFLAM && flam != FIRE_XLG)
      set_shape(x, y - 1, FIRE_LG);
  }

  if (y < 10) {
    /* Down */
    shape = shape_at(x, y + 1);
    flam = flammable(shape);
    if (flam != FIRE_INFLAM && flam != FIRE_XLG)
      set_shape(x, y + 1, FIRE_LG);
  }

  /* (Re)set explosion effect */
  exploding = 15;
}

/* Determines whether moving a given direction from
   a particular position is still in-bounds */
unsigned char valid_dir(unsigned char x, unsigned char y, unsigned char dir) {
  int dx, dy;

  dx = dir_x[dir];
  dy = dir_y[dir];
  return !((dx == -1 && x == 0) ||
      (dy == -1 && y == 0) ||
      (dx == 1 && x == 19) ||
      (dy == 1 && y == 10));
};

/* Return the flammability of an object; used to determine
   how (and if) fire spreads */
unsigned char flammable(unsigned char c) {
  if (c == OIL || c == GASLEAK_RIGHT || c == GASLEAK_LEFT || c == GASLEAK_UP || c == GASLEAK_DOWN) {
    /* Oil barrel and gas leaks cause an explosion */
    /* (Note: Unlike the others, this is not a shape drawn
       on the screen, but indicates we want an explosion) */
    return FIRE_XLG;
  } else if (c == CIVILIAN) {
    set_sound(100, 2, 0xA0, 15, 1);
    return 0;
  } else if (c == CRATE || c == CRATE_BROKEN || c == AX) {
    /* Crates and ax ignite fully */
    return FIRE_LG;
  } else if (c == 0) {
    /* Empty spaces ignite smally */
    return FIRE_SM;
  } else {
    /* Other things do not ignite */
    /* (Note: Unlike the others, this is not a shape drawn
       on the screen, but indicates fire cannot spread this
       direction) */
    return FIRE_INFLAM;
  }
}

void set_sound(char p, char pch, char dist, char vol, char volch) {
  hit_pitch = p;
  hit_pitch_change = pch;
  hit_distortion = dist;
  hit_volume = vol;
  hit_vol_decrease = volch;
}
