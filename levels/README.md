# Firefighter Levels Documentation

Creating levels for "Firefighter", a twin-stick firefighting action game
for the Atari 8-bit.

## Credits
By Bill Kendrick <bill@newbreedsoftware.com>  
http://www.newbreedsoftware.com/firefighter/

Documented 2023.08.18 - 2023.08.21

------------------------------------------------------------------------

# Level File Format

Levels are 20 blocks wide by 11 blocks tall.  The level files are
stored as ASCII text files, containing 11 lines of exactly 20 characters.

The firefighter symbol (`F`) must appear exactly once in the level.

------------------------------------------------------------------------

# Symbols:

## Immovable objects:

 * `#` (pound/number sign/hash) -- `WALL` - A brick wall
 * `E` (capital E) -- `EXIT1` - Left side of the "Exit" sign
 * `e` (lowercase e) -- `EXIT2` - Right side of the "Exit" sign
 * `D` (capital D) -- `DOOR` - A door

## Crates & Oil Barrels:
 * `C` (capital C) -- `CRATE`
 * `c` (lowercase c) -- `CRATE_BROKEN`
 * `O` (capital O) -- `OIL`

## Fire:
 * `,` (comma) -- `FIRE_SM`
 * `;` (semicolon) -- `FIRE_MD`
 * `*` (asterisk/star) -- `FIRE_LG`

## Pipes & Valves:

### Straight
 * `|` (pipe) -- `PIPE_UP_DOWN`
 * `:` (colon) -- `PIPE_BROKEN_UP_DOWN`
 * `-` (minus/dash) -- `PIPE_LEFT_RIGHT`
 * `=` (equals) -- `PIPE_BROKEN_LEFT_RIGHT`

### Curved
 * `{` (open curly brace) -- `PIPE_DOWN_RIGHT`
 * `}` (close curly brace) -- `PIPE_DOWN_LEFT`
 * `[` (open curly bracket) -- `PIPE_UP_RIGHT`
 * `]` (close square bracket) -- `PIPE_UP_LEFT`

### Valves
 * `+` (plus) -- `VALVE_OPEN`
 * `X` (capital X) -- `VALVE_CLOSED`

## Characters (indicates starting positions)
 * `F` (capital F) -- `Firefighter` (player)
 * `x` (lowercase x) -- `CIVILIAN` (workers)

## Collectibles:
 * `A` (capital A) -- `AX`

# Notes on cellular automata:
 * Gas leaks spring from broken pipes if an attached valve is open
 * Workers move on their own (randomly, or following firefighter, if nearby)
 * Fire spreads on its own

# Notes on interactive objects:
 * Ax is a collectible object
 * Oil cans can be moved by pushing into them
 * Crates and broken crates can be moved, if you *do not* have the ax
 * Crates can be broken, if you *do* have the ax
 * Broken crates can be destroyed, if you *do* have the ax
 * Regular left/right (horizontal) and up/down (vertical) pipes will
   be broken if you *do* have the ax
 * Valves can be opened/closed by pushing into them
 * Workers escape if they are adjacent to the firefighter when player pushes
   into a door or exit sign block

# Notes on flammable objects:
 * The crates, broken crates, and the ax may be destroyed by fire
   (causing large fire)
 * Oil barrels may be destroyed by fire, and cause an explosion
 * Gas leaks cause an explosion
 * Explosions are large fire in the center, plus the four adjacent
   cardinal directions (up, down, left, right), when possible

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

