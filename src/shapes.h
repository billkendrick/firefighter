/*
  Firefighter shape table

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2023-08-27
*/

#ifndef _SHAPES_H
#define _SHAPES_H

#define COLOR1 64
#define COLOR3 192

/* These correspond to the shapes found in the font
   (with highest first and/or second bits set, to
   specify the color) */

#define BLANK 0

#define PIPE_UP_DOWN (1 + COLOR1)
#define PIPE_BROKEN_UP_DOWN (2 + COLOR1)
#define PIPE_LEFT_RIGHT (3 + COLOR1)
#define PIPE_BROKEN_LEFT_RIGHT (4 + COLOR1)

#define PIPE_DOWN_RIGHT (5 + COLOR1)
#define PIPE_DOWN_LEFT (6 + COLOR1)
#define PIPE_UP_LEFT (7 + COLOR1)
#define PIPE_UP_RIGHT (8 + COLOR1)

#define VALVE_OPEN (9 + COLOR1)
#define VALVE_CLOSED (10 + COLOR1)

#define CIVILIAN (11 + COLOR1)
#define CIVILIAN_MOVED (0 + COLOR1)

#define GASLEAK_RIGHT 12
#define GASLEAK_LEFT 13
#define GASLEAK_DOWN 14
#define GASLEAK_UP 15

#define OIL (16 + COLOR1)

#define CRATE (17 + COLOR3)
#define CRATE_BROKEN (18 + COLOR3)

#define FIREFIGHTER_LEFT (19 + 128)
#define FIREFIGHTER_RIGHT (20 + 128)

#define AX (21 + COLOR1)

#define FIRE_SM 22
#define FIRE_MD 23
#define FIRE_LG 24
#define FIRE_INFLAM 254 /* Special meaning; not an actual character */
#define FIRE_XLG 255 /* Special meaning; not an actual character */

#define WALL (25 + COLOR3)

#define DOOR (26 + COLOR3)
#define EXIT1 (27 + COLOR3)
#define EXIT2 (28 + COLOR3)

#endif
