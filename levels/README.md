# Firefighter Levels Documentation

Creating levels for "Firefighter", a twin-stick firefighting action game
for the Atari 8-bit.

## Credits
By Bill Kendrick <bill@newbreedsoftware.com>  
http://www.newbreedsoftware.com/firefighter/

Documented 2023-08-18 - 2025-03-19

------------------------------------------------------------------------

# Level File Format

Levels are 20 blocks wide by 11 blocks tall.  The level files are
stored as ASCII text files, containing 11 lines of exactly 20 characters.

The firefighter symbol (`F`) must appear exactly once in the level,
and denotes the starting position of the player.

Be sure to include at least one fire symbol (small `,`, medium `;`, or
large `*`), and one or more of the exit symbols (`E` and `e` for the
"EXIT" sign, and/or `D` for the door).

------------------------------------------------------------------------

# Symbols:

Below you'll see character you may place in a level file (and its name,
for clarity), along with the value used in the code (see
[`shapes.h`](../src/shapes.h) and
[`level_to_dat.php`](../tools/level_to_dat.php)), along with a brief
description.

## Immovable objects:

 * `#` (pound/number sign/hash) -- `WALL` - A brick wall
 * `E` (capital E) -- `EXIT1` - Left side of the "Exit" sign (goal)
 * `e` (lowercase e) -- `EXIT2` - Right side of the "Exit" sign (goal)
 * `D` (capital D) -- `DOOR` - A door (goal)

## Crates & Oil Barrels:

 * `C` (capital C) -- `CRATE` - Wooden crates (may be pushed, or broken by an ax); can burn
 * `c` (lowercase c) -- `CRATE_BROKEN` - Broken wooden creates (may be pushed, or destroyed by an ax); can burn
 * `O` (capital O) -- `OIL` - Oil barrels (may be pushed); explode on contact with fire

## Fire:

 * `,` (comma) -- `FIRE_SM` - Small fire; grows to medium fire
 * `;` (semicolon) -- `FIRE_MD` - Medium fire; grows to large fire
 * `*` (asterisk/star) -- `FIRE_LG` - Large fire; spreads around the room

## Pipes & Valves:

### Straight

 * `|` (pipe) -- `PIPE_UP_DOWN` - Vertical pipe (may be broken by ax)
 * `:` (colon) -- `PIPE_BROKEN_UP_DOWN` - Broken vertical pipe (leaks explosive gas)
 * `-` (minus/dash) -- `PIPE_LEFT_RIGHT` - Horizontal pipe (may be broken by ax)
 * `=` (equals) -- `PIPE_BROKEN_LEFT_RIGHT` - Broken horizontal pipe (leaks explosive gas)

### Curved

 * `{` (open curly brace) -- `PIPE_DOWN_RIGHT` - Pipe corner, connecting down to right (┌)
 * `}` (close curly brace) -- `PIPE_DOWN_LEFT` - Pipe corner, connecting down to left (┐)
 * `[` (open curly bracket) -- `PIPE_UP_RIGHT` - Pipe corner, connecting up to right (└)
 * `]` (close square bracket) -- `PIPE_UP_LEFT` - Pipe corner, connecting up to left (┘)

### Valves

 * `+` (plus) -- `VALVE_OPEN` - Open pipe valve (may be closed); causes broken pipes to leak gas
 * `X` (capital X) -- `VALVE_CLOSED` - Closed pipe valve (may be opened)

## People

 * `F` (capital F) -- Firefighter - Indicates player's starting position
 * `x` (lowercase x) -- `CIVILIAN` - Indicate workers' starting positions

## Collectibles:

 * `A` (capital A) -- `AX` - Once collected, crates & pipes may be broken (can burn)

------------------------------------------------------------------------

# Notes:

## Cellular automaton:

Each frame of the game loop, the screen (technically, half of it) is
processed by a cellular automaton routine that brings live to objects
on the screen.  (So for example, every blank space that fire can reach
may become engulfed; flames are not stored as an array of individual
objects that get placed on the screen.  The screen data itself *is* the
game.)

 * Gas leaks spring from broken pipes if *any* valves on screen are open
 * Workers move on their own (randomly, or following firefighter, if nearby)
 * Fire spreads on its own

# Interactive objects:

 * Ax is a collectible object
 * Oil cans can be moved by pushing into them
 * Workers cans can be moved by pushing into them
 * Crates and broken crates can be moved, if you *do not* have the ax
 * Crates can be broken, if you *do* have the ax
 * Broken crates can be destroyed, if you *do* have the ax
 * Regular left/right (horizontal) and up/down (vertical) pipes will
   be broken if you *do* have the ax
 * Pipes can be broken, if you have the ax
 * Valves can be opened/closed by pushing into them
 * Workers escape if they are adjacent to the firefighter when player pushes
   into a door or exit sign block

## Flammable objects:

 * The crates, broken crates, and the ax may be destroyed by fire
   (causing large fire)
 * Oil barrels may be destroyed by fire, and cause an explosion
 * Gas leaks cause an explosion
 * Explosions are large fire in the center, plus the four adjacent
   cardinal directions (up, down, left, right), when possible

------------------------------------------------------------------------

# Example Level:

```
##################Ee
#A     C      x  ,*D
#      C       x  ;#
#      #         ,c#
#CCCCCC#          c#
########         cC#
#                  #
#  #               #
#  #          ######
F  #          #OOOO 
###############     
```

There is a [level template](LEVEL-TEMPLATE.txt) you can use as a starting
point.

------------------------------------------------------------------------

# Building Levels:

## Text to Data

All level text files (`levelNNN.txt`) in this directory are read,
converted to binary data, and assembled into a single file,
`data/levels.dat`, by the [`level_to_dat.php`](../tools/level_to_dat.php)
script.

This is handled by the `make` process (via the [`Makefile`](../Makefile)).
Note: The `LEVEL_FILES` variable in the `Makefile` is used to specify which
files to include.  In version `0.1-BETA10`, for example, there were only
12 levels, despite there being level files up to `level050.txt`; 13 through 50
were placeholders.

## Compression

Then, that file is read and the simple compression process
(a form of [run-length encoding (RLE)](https://en.wikipedia.org/wiki/Run-length_encoding))
is applied by the [`level_compress.php`](../tools/level_compress.php) script.
This results in `data/levels_cmp.dat`, which is what's built
into the final executable program.

(This step, too, is handled by the `make` process & `Makefile`.)
