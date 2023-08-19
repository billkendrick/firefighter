/*
  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/

  2023-08-13 - 2023-08-17
*/

#ifndef _SHAPES_H
#define _SHAPES_H

#define PIPE_UP_DOWN (1 + 64)
#define PIPE_BROKEN_UP_DOWN (2 + 64)
#define PIPE_LEFT_RIGHT (3 + 64)
#define PIPE_BROKEN_LEFT_RIGHT (4 + 64)

#define PIPE_DOWN_RIGHT (5 + 64)
#define PIPE_DOWN_LEFT (6 + 64)
#define PIPE_UP_LEFT (7 + 64)
#define PIPE_UP_RIGHT (8 + 64)

#define VALVE_OPEN (9 + 64)
#define VALVE_CLOSED (10 + 64)

#define CIVILIAN (11 + 64)

#define GASLEAK_RIGHT 12
#define GASLEAK_LEFT 13
#define GASLEAK_DOWN 14
#define GASLEAK_UP 15

#define OIL (16 + 64)

#define CRATE (17 + 192)
#define CRATE_BROKEN (18 + 192)

#define FIREMAN_LEFT (19 + 128)
#define FIREMAN_RIGHT (20 + 128)

#define AX (21 + 64)

#define FIRE_SM 22
#define FIRE_MD 23
#define FIRE_LG 24
#define FIRE_INFLAM 254 /* Special meaning; not an actual character */
#define FIRE_XLG 255 /* Special meaning; not an actual character */

#define DOOR (26 + 192)

#define EXIT1 (27 + 192)
#define EXIT2 (28 + 192)

#define WALL (25 + 192)

#endif
