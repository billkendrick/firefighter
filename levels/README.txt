Firefighter Levels README
2023.08.18

Levels are 20 wide by 10 tall.
Symbols:

Immovable objects:
 # - WALL
 E - EXIT1 (left side)
 e - EXIT2 (right side)
 D - DOOR

Crates & Oil Barrels:
 C - CRATE
 c - CRATE_BROKEN
 O - OIL

Fire:
 , - FIRE_SM
 ; - FIRE_MD
 * - FIRE_LG

Pipes & Valves:
 | - PIPE_UP_DOWN
 : - PIPE_BROKEN_UP_DOWN
 - - PIPE_LEFT_RIGHT
 = - PIPE_BROKEN_LEFT_RIGHT
 [TBD] - PIPE_DOWN_RIGHT
 [TBD] - PIPE_DOWN_LEFT
 [TBD] - PIPE_UP_LEFT
 [TBD] - PIPE_UP_RIGHT
 + - VALVE_OPEN
 X - VALVE_CLOSED

Characters (indicates starting positions)
 F - Fireman (player)
 x - CIVILIAN

Ax:
 A - AX

Notes on cellular automata:
 * Gas leaks spring from broken pipes if an attached valve is open
 * Civilians move on their own (randomly, or following fireman, if nearby)
 * Fire spreads on its own

Notes on interactive objects:
 * Ax is a collectible object
 * Oil cans can be moved by pushing into them
 * Crates and broken crates can be moved, if you do NOT have the ax
 * Crates can be broken, if you DO have the ax
 * Broken crates can be destroyed, if you DO have the ax
 * Regular left/right (horizontal) and up/down (vertical) pipes will
   be broken if you DO have the ax
 * Valves can be opened/closed by pushing into them
 * Civilians escape if they are adjacent to the fireman when player pushes
   into a door or exit sign block

Notes on flammable objects:
 * The crates, broken crates, and the ax may be destroyed by fire
   (causing large fire)
 * Oil barrels may be destroyed by fire, and cause an explosion
 * Gas leaks cause an explosion
 * Explosions are large fire in the center, plus the four adjacent
   cardinal directions (up, down, left, right), when possible

