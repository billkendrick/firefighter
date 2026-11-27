/*
  Firefighter shape table

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  https://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2026-11-27
*/

#ifndef _SHAPES_H
#define _SHAPES_H

#define COLOR3 128

/* These correspond to the shapes found in the font.
   (Fire DLI effect happens to COLPF3, characters
   with the highest ("inverse video") bit set) */

#define BLANK 0

#define PIPE_UP_DOWN 1
#define PIPE_BROKEN_UP_DOWN 2
#define PIPE_LEFT_RIGHT 3
#define PIPE_BROKEN_LEFT_RIGHT 4

#define PIPE_DOWN_RIGHT 5
#define PIPE_DOWN_LEFT 6
#define PIPE_UP_LEFT 7
#define PIPE_UP_RIGHT 8

#define VALVE_OPEN 9
#define VALVE_CLOSED 10

#define CIVILIAN 11
#define CIVILIAN_MOVED 128 /* Special value for a moved civilian's location */

#define GASLEAK_RIGHT (12 + COLOR3)
#define GASLEAK_LEFT (13 + COLOR3)
#define GASLEAK_DOWN (14 + COLOR3)
#define GASLEAK_UP (15 + COLOR3)

#define OIL 16

#define CRATE 17
#define CRATE_BROKEN 18

#define FIREFIGHTER_LEFT 19
#define FIREFIGHTER_RIGHT 20

#define AX 21

#define FIRE_SM (22 + COLOR3)
#define FIRE_MD (23 + COLOR3)
#define FIRE_LG (24 + COLOR3)
#define FIRE_INFLAM 254 /* Special meaning; not an actual character */
#define FIRE_XLG 255 /* Special meaning; not an actual character */

#define WALL 25

#define DOOR 26
#define EXIT1 27
#define EXIT2 28

#endif
