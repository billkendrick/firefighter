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

/* These correspond to the left half of each shapes found
   in the font.
   (Fire DLI effect happens to COLPF3, characters
   with the highest ("inverse video") bit set) */

#define BLANK 0

#define PIPE_UP_DOWN 2
#define PIPE_BROKEN_UP_DOWN 4
#define PIPE_LEFT_RIGHT 6
#define PIPE_BROKEN_LEFT_RIGHT 8

#define PIPE_DOWN_RIGHT 10
#define PIPE_DOWN_LEFT 12
#define PIPE_UP_LEFT 14
#define PIPE_UP_RIGHT 16

#define VALVE_OPEN 18
#define VALVE_CLOSED 20

#define CIVILIAN 22
#define CIVILIAN_MOVED 128 /* Special value for a moved civilian's location */

#define GASLEAK_RIGHT (24 + COLOR3)
#define GASLEAK_LEFT (26 + COLOR3)
#define GASLEAK_DOWN (28 + COLOR3)
#define GASLEAK_UP (30 + COLOR3)

#define OIL 32

#define CRATE 34
#define CRATE_BROKEN 36

#define FIREFIGHTER_LEFT 38
#define FIREFIGHTER_RIGHT 40

#define AX 42

#define FIRE_SM (44 + COLOR3)
#define FIRE_MD (46 + COLOR3)
#define FIRE_LG (48 + COLOR3)
#define FIRE_INFLAM 254 /* Special meaning; not an actual character */
#define FIRE_XLG 255 /* Special meaning; not an actual character */

#define WALL 50

#define DOOR 52
#define EXIT1 54
#define EXIT2 56

#define WATER_N  70
#define WATER_NW 120
#define WATER_W  78
#define WATER_SW 104
#define WATER_S  80
#define WATER_SE 96
#define WATER_E  88
#define WATER_NE 112

#endif
