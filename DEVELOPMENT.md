# Firefighter Development Docs

Development details of "Firefighter", a twin-stick firefighting action
game for the Atari 8-bit.

## Credits
By Bill Kendrick <bill@newbreedsoftware.com>  
http://www.newbreedsoftware.com/firefighter/

Developed 2023-08-13 - 2023-08-22

------------------------------------------------------------------------

This game is written (almost) entirely in the C programming language,
and cross-compiled using the [CC65](https://cc65.github.io/)
cross development package for 6502-based systems, such as the Atari 8-bit.

## Inspiration

In 2019 my children were playing a lot of _Real Heroes: Firefighter_ on
the Wii.  In the game you navigate (in a 3D first-person perspective), equip
yourself with tools (ax, hose, etc.), put out fires, and save civilians.
It inspired me to design a game with similar concepts, but as a top-down
twin-stick "shooter", like _Robotron: 2084_.  I have a very nice arcade-style
controller (from Edladdin Controllers) that I use for the Atari 8-bit port
of that game, and needed an excuse to write a twin-stick game.
(Also, this isn't my fire firefighting game; as a kid back in the 1980s I wrote
a game in Atari BASIC where you use a helicopter to put out a fire on a skyscraper.)

Four years later I happened across my copious notes (including some hand-drawn
pixel art) and decided to finally take a stab at it.  I fired up _Envision_
font editor on Linux, and immediately spent too much time designing the shape of
the text characters used in the game, based on the fonts seen on the sides of
firetrucks in the US.  I finally also began adding the in-game objects.

## Graphics

I opted to use ANTIC mode 7 (aka `GRAPHICS 2`), the mode with very large text
(20 characters tall by 12 characters wide), where each character is a single
color, with one of our colors possible, depending on the two high-bits of the
screen data.  This means only 64 shapes are available.  No inverse-video
(inverted pixels) are possible, either.  (Compare to regular 40 by 24 text mode
(aka `GRAHICS 0`), which allows for 128 shapes, plus inverse-video versions.)

However, when setting the `CHBASE` register (or the OS's `CHBAS` shadow) to
tell the Atari where in memory to look for character graphics (the "font"),
it is possible to offset the character set page by two, which causes the
ANTIC 7 parts of the screen to use the *latter* half of the character set.
The small text, ANTIC mode 2 (aka `GRAPHICS 0`) continues to use the full set.
Therefore, while I'm limited to only uppercase characters the small mode 2 text,
and no text at all (only game tile shapes) in the large mode 7 text, this means
I can get away with only a *single* 1KB character set for all of my needs!

In reality, I have two character sets, and flip between them every few frames
(screen refreshes) to get some animation -- fire burning, gas leaks and
water spraying, and workers flailng their limbs.  So in the end, 2KB of space
is used for all of the character data.  (See the `fire1.fnt` and `fire2.fnt`
files in the [`fonts/`](fonts/) subdirectory.)

You'll notice large mode 7 text used to display the "FIREFIGHTER" title at the
top (and other messages between levels during the game).  This is acheived by
_not_ offsetting the character set at first.  Instead, it happens during a
Display List Interrupt (DLI), which also changes some colors in the
palette, including handling the fire and gas leak color cycling.
The font animation happens here, as well -- so any game loop slowdown that may
happen has no effect on these things.

One byte in Page 6 is used to tell the DLI where the font's base is (there may
be a better way of handling this?), and it uses the `RTCLOK` timer register,
updated by the OS each Vertical Blank Interrupt (VBI) to animate.
(Note: I don't use any of my own VBIs in this game.)

Another byte in Page 6 is used to let the DLI change the background color of
the game area, based on events (e.g., flashing when an oil barrel explodes).
(See [`dli.c`](src/dli.c).  Note: This is the only hand-coded
6502 assembly language in this game.  Everything else is in C.)

## Sound

### Title screen

The title screen has a timer that increments each frame.  During some
parts of its count, it will play an oscillating siren tone that
increases in volume (and pitch), then decreases in volume (and pitch).
(Yes, I made a vague attemt at a "firetruck speeding by" Doppler effect.)

Easter egg: If you push either stick in any direction on the title
screen, you can cause a firetruck horn blaring noise while the sirens
are audible!

### General Game Sounds

Various game sounds are played on one of the Atari's four "voices",
and is handled by a number of variables used to set the pitch,
initial volume, pitch change (if any), and speed of volume decrease.
The pitch (if applicable) and volume change once per game loop.

This system is used for the majority of events, which are:

 * Collecting the ax
 * Pushing crates/oil barrels
 * Breaking/destroying crates
 * Damaging pipes
 * Opening/closing gas pipeline valves
 * Rescuing workers & workers perishing

### Water Spray Sound

This is a simple high-pitched hiss sound played on a dedicated
"voice" whenever the joystick is being used to (attempt to) spray
water in any direction.

### Fire Crackling Sound

The fire sound is a random, low-pitched sound played on its own dedicated
"voice", and its volume is based on how much fire is on the screen.
(A counter ticks up each time fire is observed during the cellular
automaton routine.  That routine actually sets the sound registers.)

### Explosion Sound

This is a totally random noise sound played on its own dedicated
"voice", controlled by a countdown timer that sets its volume.

## Gameplay

### Cellular Automaton

The game is driven by a
[cellular Automaton](https://en.wikipedia.org/wiki/Cellular_automaton)
routine.  The screen memory of the game area is treated as individual
cells.  Certain kinds of cells have behaviors which can affect their
cell and/or the cells next to them.

#### Fire

For example, a cell that contains a small fire has a random chance
of growing into a medium fire.  Similarly, a medium fire has a chance
to become a large fire.  A cell containing a large fire has a random
chance of spreading in a random direction.

#### Workers

The workers are also treated as cells.  They "move" by placing a blank
shape in their current cell, and a worker in an adjacent cell.
They first observe nearby adjacent cells to see whether the firefighter
is nearby.  If so, they will go that direction; otherwise, they will
pick a random direction.

#### Gas Leaks

Whenever broken pipes are observed, a counter is checked that
keeps track of how many valves are open on the screen.  If one or
more is open, then gas will leak in any blank spots adjacent to
broken pipes.  If no valves are open, any gas leaks will be removed.

#### Water

During the cellular automaton phase, any water on the level is
erased.  This means the main loop only needs to worry about
drawing it while the joystick is being pushed.

### Water Spray

The player may spray water in any of eight directions.
Each direction is made up of four different tile shapes in the
character set (so we're using half of the game tile portion of
our character set just for water spray!  It *is* an important
aspect of the game, however!)

The sprays consists of a narrow point placed directly adjacent
to the firefighter, plus three more that represent the wide end
of the spray.  For the four cardinal directions, the wide spray
is drawn as three vertical or three horizontal shapes in a row.
For the four diagonal directions, the middle of the wide part
continues in the direction of spray, and the two ends appear
perpendicular to it (forming a 2x2 square of shapes).

Examples:

```
.....  .....  .....
.../.  .\-/.  ..|\.
.F<|.  ..V..  ..L-.
...\.  ..F..  .F...
.....  .....  .....
```

If the initial (narrow, adjacent) shape hits an obstacle, or
fire, the other three shapes will not be drawn (and, hence, not
have any affect on fire beyond the position adjacent to the player).

## Levels

Levels are created as plain ASCII text files containing special
(human-readable) characters that represent the handful of various
shapes possible in the game.  (While we have 64 shapes in our character
set to work with, in up to 4 colors each, in reality many shapes are
not part of the map.

See the [levels documentation](levels/README.md) for details.
