/*
  Firefighter shape table

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-27 - 2023-08-27
*/

#include "shapes.h"

/* Compressed levels only record the low 6 bits of
   each shape, so we need to map back to the actual
   shapes we use (which include color data in the
   high two bits; see above) */

unsigned char level_data_to_screen[29] = {
  BLANK,

  PIPE_UP_DOWN,
  PIPE_BROKEN_UP_DOWN,
  PIPE_LEFT_RIGHT,
  PIPE_BROKEN_LEFT_RIGHT,

  PIPE_DOWN_RIGHT,
  PIPE_DOWN_LEFT,
  PIPE_UP_LEFT,
  PIPE_UP_RIGHT,

  VALVE_OPEN,
  VALVE_CLOSED,

  CIVILIAN,

  GASLEAK_RIGHT,
  GASLEAK_LEFT,
  GASLEAK_DOWN,
  GASLEAK_UP,

  OIL,

  CRATE,
  CRATE_BROKEN,

  FIREFIGHTER_LEFT,
  FIREFIGHTER_RIGHT,

  AX,

  FIRE_SM,
  FIRE_MD,
  FIRE_LG,

  WALL,

  DOOR,
  EXIT1,
  EXIT2,
};
