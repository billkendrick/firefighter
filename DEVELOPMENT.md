# Firefighter Development Docs

Development details of "Firefighter", a twin-stick firefighting action
game for the Atari 8-bit.

## Credits
By Bill Kendrick <bill@newbreedsoftware.com>  
http://www.newbreedsoftware.com/firefighter/

Developed 2023-08-13 - 2025-02-20

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
(Also, this isn't my first fire firefighting game; as a kid back in the
1980s I wrote a game in Atari BASIC where you use a helicopter to put
out a fire on a skyscraper.)

Four years later I happened across my copious notes (including some hand-drawn
pixel art) and decided to finally take a stab at it.  Scans of, and notes about,
the [original design docs](design-docs/) are available.

I fired up _Envision_ font editor on Linux, and immediately spent too
much time designing the shape of the text characters used in the game,
based on the fonts seen on the sides of firetrucks in the US.  I finally
also began adding the in-game objects.

## Graphics

### Tile Graphics

Originally, I opted to use mode 7 of the Atari's ANTIC graphics processor
(aka `GRAPHICS 2`).  This mode generates very large text -- 20 characters wide
by 12 characters tall -- with each character being 8x8 pixels of a single color,
with one of four colors possible, depending on the two high-bits of the screen data.

Since 256 ÷ 4 = 64, this means only 64 shapes (of the 128 possible in a charater
set) are available to use in a mode 7 line.  (No inverse-video (inverted pixels)
are possible, either.  Compare this to ANTIC mode 2, the regular 40 by 24 text mode
(aka `GRAHICS 0`), which allows for 128 shapes, plus inverse-video versions,
but no colors.)

However, when setting the ANTIC chip's `CHBASE` register (or the OS's `CHBAS` shadow
register) to tell the Atari where in memory to look for character graphics (the "font"),
it is possible to offset the character set page by two, which causes the
ANTIC 7 (as well as ANTIC 6, aka `GRAPHICS 1`) parts of the screen to use
the *latter* half of the character set.  The small text modes, such as
ANTIC mode 2 (aka `GRAPHICS 0`), continue to use the full character set.

Therefore, in the earlier releases of the game, although I was limited to only
uppercase characters for the small mode 2 text, and no alphanumeric characters at all
(only game tile shapes) in the large mode 7 text, this meant I was able to get
away with only a *single* 1KB character set for all of my needs!  (Actually, a pair
of them; see below.)

### Higher Resolution Via ANTIC Tricks

However, I later decided to change the game screen to ANTIC mode 6 (aka `GRAPHICS 1`),
which is very similar but each line is half as tall, resulting in twice as
many lines (i.e., a screen that is 20 characters wide by 24 characters tall).
By using *two* rows of text per row of the game map, this allowed for the
shapes to be composed to twice as many pixels: 8x16 instead of 8x8.

Since this requires twice as many characters, but only 64 are available,
I use Display List Interrupt (DLI) routines, along with *two* character
sets -- one for the top half of the tile shapes, and one for the bottom -- and
switching between them every other row.

Also, twice as many rows of tiles will require twice as much screen memory, as
well as having to write two bytes every time a shape must be drawn onto the map.
However, the characters on even rows must be identical to the characters
on the odd rows above them, for the two halves of the tiles to appear
correctly; they must be the two halves of the *same* tile.

This allowed me to take advantage of the ANTIC Display List's "Load Memory Scan"
(LMS) instruction, instructing it to reload the same row of screen data
every other line.  In other words, every row of the map data appears twice in a
row on the screen: rows 1 and 2 of the screen both show row 1 of the map; rows
3 and 4 of the screen both show row 2 of the map; etc.

By combining alternating character sets (via DLI) and repeating screen
data (via LMS), every odd row shows the "top half" of a tile shape, and
every even row shows the "bottom half".  Only 20 * 11 bytes of screen
memory are needed, and the game logic continues to only need concern itself
with a map that's 20x11 tiles in size.

The Atari's powerful ANTIC graphics chip does the heavy lifting to display
the tile shapes with greater detail than a plain ANTIC mode 7 display could.
Basically, the game went from looking like it was made of 160x96 pixels, to
looking like it was 160x192!

You might notice that the large mode 7 text used to display the "FIREFIGHTER"
title at the top (and other messages between levels during the game) also uses
the game's special font, but alphanumeric symbols appear (rather than random
tile shapes from the game).  This is acheived by _not_ offsetting the character
set at first; it first happens during the initial DLI routine. (That DLI routine
also also changes some colors in the palette.)

### Display List Interrupt (DLI) Chaining (and Rainbow Fire)

The game in fact contains three DLI routines; the first one changes the color
palette and changes the character set, and then chains to a second
DLI routine (by altering the Display List Interrupt Vector, `VDSLST`).
The second DLI routine set the character set to the one containing the top half
of the tile set, and also alters one of the color registers every few scan lines,
to create the "Atari Rainbow" effect for the fire and gas leaks.  It then chains
to a third DLI routine, which is nearly identical to the second one, but with
the bottom half of the tile set.  It chains back to the second DLI routine.

In reality, I have two sets of tile shapes, and flip between them every
few frames (screen refreshes) to get some animation -- fire burning,
gas leaks and water spraying, and workers flailng their limbs.
(See the files and documentation in the [`fonts/`](fonts/) subdirectory.)

### Animation and Color Palette

The font animation is handled during a Vertical Blank Interurpt (VBI) routine,
meaning any game loop slowdown that may happen will have no effect on the
animation effect.  It does this by setting some bytes (in Page 6 memory)
which the DLI routines use to decide which character sets to display.
It also resets the DLI Vector to point to the first DLI.

Another byte in Page 6 is used to let the DLI change the background color of
the game area, based on events (e.g., flashing when an oil barrel explodes).
(See [`dli.c`](src/dli.c).  Note: This is among the only hand-coded
6502 assembly language in this game.  Almost everything else is in straight C.)

Note: Currently, no Player/Missile Graphics ("sprites") are used in the game.
(They are used in the splash screen.)

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

### Footsteps

The footstep sound is handled separately from the POKEY chip voices,
and is instead generated by the GTIA chip (so on an Atari 400 or 800
the sound would come from the internal speaker, rather than the
TV or monitor).

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
chance of spreading (creating a new small fire) in a random direction.

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

### Level Creation

Levels are created as plain ASCII text files containing special
(human-readable) characters that represent the handful of various
shapes possible in the game.

See the [levels documentation](levels/README.md) for details.

The script [`level_to_dat.php`](tools/level_to_dat.php) reads the
individual ASCII text files in the [`levels/`](levels/) directory and
generates a binary file containing the game's internal representation
(aka the screen character bytes) of all of the levels.  (A single byte
at the beginning indicates how many levels there are, and two bytes
at the end of each level's data indicates where the firefighter's
starting position is to be.)

### Level Compression

On the screen shapes that appear also include color data in their highest
two bits).  However, we never use the same shape in multiple colors;
e.g., pipes are always green, brick walls and boxes are always red, etc.

Also, while we have 64 shapes in our character set to work with
(in up to 4 colors each) in reality many shapes are not part of the
level map.  (Half of the 32 characters in the set are used for the
various directions of water spray shapes!)

Therefore, we can strip the highest bits of the level data,
and use those bits to indicate how many times the characters
repeat (once, twice, three times, four times, etc.).  Since we often
have the same character appear many times in a row (e.g., a row of
bricks, or a large blank area in a room), this allows us to store
one byte in the place of multiple bytes for the same level layout.

The script [`level_compress.php`](tools/level_compress.php) reads
the binary level data file (created by the tool above), which in
early beta versions was used directly (and level data in the game
executable was simply copied directly to screen memory) and
creates a compressed file.  (As before, a single byte at the
beginning indicates how many levels there are.  That is followed
by four-byte sequences for each level, indicating the firefighter's
starting position, and the offset within the compressed level data
that each level starts -- they are of course different lengths!)

## High Score

The game records a top score, and allows the user to enter their
initials at the end of the game.  The disk-based version provides
a top-10 high score table, which it saves to disk.

Code exists (in [`score.c`](src/score.c)) to use standard C file
input/output (I/O) functions -- `fopen()`, `fread()`, etc. --
to write to and read from a simple high score file, "`highscor.dat`".

However, by default the game stores the high score table directly
onto a pre-defined sector of the disk.  This means there's no "file"
attached to the data, however, it allow one to easily extract the
data from the disk image.  (You always look at the same sector,
in this case 720, and don't need to worry about it moving around
between releases and dealing with the filesystem; the
Volume Table of Contents (VTOC).)

Why? So the high score table may be "shared" by many players
over the Internet! If the game disk image is made available to
users who use a [FujiNet](https://fujinet.online/) device, the
TNFS server hosting it can be told to grant players write-access
to *just* the sector containing the high score table (the rest
of the disk image will be read-only).

This is implemented using three unused bytes in
the ATR disk image file format's header.  The script
[`high_score_atr.php`](tools/high_score_atr.php) handles this modification.
(See the FujiNet project's
["High Score storage for Legacy Games" page](https://github.com/FujiNetWIFI/fujinet-platformio/wiki/High-Score-storage-for-Legacy-Games)
for further information.)

## Help Screen

The disk version offers a help screen (press `[?]` or `[Help]`
on the title screen).  This contains exactly the same content
as [`README.md`](README.md), but it has been converted from
Markdown to 40 column plain ATASCII text.

This is done using the [`markdown`](https://daringfireball.net/projects/markdown/)
tool to convert to HTML, then the [`w3m`](https://w3m.sourceforge.net/)
text-based web browser, in `-dump` mode, to convert to 40-column text,
and finally the [`tr`](https://www.gnu.org/software/coreutils/tr) tool
to convert Unix end-of-line (EOL) characters to ATASCII.

Although the help text is read from a file on disk, "Firefighter"
allows you to paginate backwards; that is, navigate to previous pages.
The "note" and "point" DOS (disk operating system) commands are utilized
to make this possible.  As each new page is read, the starting location
of that part of the text is recorded in an array, via "note".  If you
want to go to a previous page, the "point" operation is used to seek
to that part of the file, and that page (screen-full) of text is re-read.

## Splash Screen

The splash screen (disk version) is comprised of a pair of public
domain images, and text rendered in two fonts released under the SIL
Open Font License.  They can be found in the [`img-src` folder](img-src/).

They are assembled (composited), text is rendered, and they are scaled
down in both size and color palette (remapped) using
[ImageMagick](https://imagemagick.org/).

The resulting file, a [NetPBM](https://netpbm.sourceforge.net/)
Portable Gray Map (PGM), is converted to binary data suitable
for display in Atari GTIA "`GRAPHICS 9`" 16-greyscale mode (80x192 pixels)
by the script [`pgm2gr9.php`](tools/pgm2gr9.php).

Additionally, the darkest pixels from the image are extracted
to be used as the firefighter's silhouette, and stored in a
binary data file.  It is used to create a set of Player/Missile
Graphics (PMG) objects (aka "sprites") to add a 17th color to the
splash screen.

A tiny program acts as the splash screen, loading and displaying
this picture.  I take advantage of MyDOS's ability to chain
multiple "autorun" programs (aka `AUTORUN.SYS` in Atari DOS) by
having it load the splash first (`SPLASH.AR0`), followed by
the game itself second (`FIREFITE.AR1`).

