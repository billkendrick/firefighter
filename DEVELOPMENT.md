# Firefighter Development Docs

Development details of "Firefighter", a twin-stick firefighting action
game for the Atari 8-bit.

## Credits
By Bill Kendrick <bill@newbreedsoftware.com>  
https://www.newbreedsoftware.com/firefighter/

Developed 2023-08-13 - 2026-11-13

------------------------------------------------------------------------

This game is written (almost) entirely in the C programming language,
and cross-compiled using the [CC65](https://cc65.github.io/)
cross development package for 6502-based systems, such as the Atari 8-bit.

## Inspiration

In 2019 my children were playing a lot of _Real Heroes: Firefighter_ on
the Nintendo Wii game console.  In the game you navigate (in a 3D
first-person perspective), equip yourself with tools (ax, hose, etc.),
put out fires, and save civilians.  It inspired me to design a game with
similar concepts, but as a top-down twin-stick "shooter", like
[_Robotron: 2084_](https://en.wikipedia.org/wiki/Robotron:_2084).
I have a very nice arcade-style controller (from Edladdin Controllers) that I
use for the Atari 8-bit computer and Atari 7800 ports of that game,
and wanted an excuse to write a twin-stick game. (Also, this isn't my
first fire firefighting game; as a kid back in the 1980s I wrote a game
in Atari BASIC where you use a helicopter to put out a fire on a skyscraper.)

Four years later (in 2023) I happened across my copious notes (including some
hand-drawn pixel art) and decided to finally take a stab at it.  Scans of,
and notes about, the [original design docs](design-docs/) are available.

I fired up [_Envision_ font editor](https://atari.miribilist.com/envision/)
on Linux, and immediately spent too much time designing the shape of the
text characters used in the game, based on the fonts seen on the sides of
firetrucks in the US.  I finally also began adding the in-game objects.

## Graphics

### Tile Graphics

Originally (as seen in the first few beta versions of the game), I opted to use
mode 7 of the Atari's [ANTIC graphics processor](https://en.wikipedia.org/wiki/ANTIC),
aka `GRAPHICS 2` in Atari BASIC.

This mode generates very large text -- 20 characters wide by 12 characters tall --
with each character being 8×8 pixels of a single color.  That totals 160×96 pixels
on the screen, and hence an appearance similar to ANTIC mode 13 ($D), aka
`GRAPHICS 7`.  Each character may be one of four colors, based on the two high-bits
of the screen data.

Since 256 ÷ 4 = 64, this means only 64 shapes (of the 128 possible in a charater
set) are available to use in a mode 7 line.  (No inverse-video (inverted pixels)
are possible, either.  Compare this to ANTIC mode 2, the regular 40 by 24 text mode
(aka `GRAHICS 0`), which allows for 128 shapes, plus inverse-video versions,
but characters may only be a single color.)

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
of them, for animation; see below.)

### Higher Resolution Via ANTIC Tricks

However, I later decided to change the game screen to ANTIC mode 6 (aka `GRAPHICS 1`),
which is very similar but each line is half as tall, resulting in twice as
many lines (i.e., a screen that is 20 characters wide by 24 characters tall).
By using *two* rows of text per row of the game map, this allowed for the
shapes to be composed of twice as many pixels: 8×16 instead of 8×8.
(This gives a 160×192 pixel appearance, similar to ANTIC mode 14 ($E),
known in the Atari 400/800 era as `GRAPHICS 7+` or `GRAPHICS 7½`, and
in the XL/XE era available as `GRAPHICS 15`.)

Since this requires twice as many characters, but only 64 are available, I use
[Display List Interrupt (DLI)](https://www.atariarchives.org/search.php?q=display+list+interrupts)
routines, along with *two* character sets -- one for the top half of the tile
shapes, and one for the bottom -- and switch between them every other row.

Twice as many rows of tiles should require twice as much screen memory, and
the game would have to write two bytes every time a shape must be drawn onto
the screen.  However, keep in mind that the characters on even rows must be
identical to the characters on the odd rows above them (for the two halves of
the tiles to appear correctly) -- they must be the two halves of the *same* tile.

This allowed me to take advantage of the ANTIC Display List's
["Load Memory Scan" (LMS) instruction](https://en.wikipedia.org/wiki/ANTIC#Load_memory_scan),
instructing it to reload the same row of screen data every other line.  In other
words, every row of the map data appears twice in a row on the screen: rows 1
and 2 of the screen both show row 1 of the map; rows 3 and 4 of the screen both
show row 2 of the map; etc.

By combining alternating character sets (via DLI) and repeating screen
data (via LMS), every odd row shows the "top half" of a tile shape, and
every even row shows the "bottom half".  Only 20 * 11 = 220 bytes of screen
memory are needed, and the game logic continues to only need concern itself
with a map that's 20×11 tiles in size.  (Note: In _Firefighter_, the game
area is only 11 tiles high; there is a title and status line at the top of
the screen.)

The Atari's powerful ANTIC graphics chip does the heavy lifting to display
the tile shapes with greater detail than a plain ANTIC mode 7 display could.
Basically, the game went from looking like it was made of 160×96 pixels, to
looking like it was 160×192!

You might notice that the large mode 7 text used to display the "FIREFIGHTER"
title at the top (and other messages between levels during the game) also uses
the game's special font, but alphanumeric symbols appear (rather than random
tile shapes from the game).  This is acheived by _not_ offsetting the character
set at first; it first happens during the initial DLI routine. (That DLI routine
also also changes some colors in the palette, at the top of the game area.)

### Display List Interrupt (DLI) Chaining (and Rainbow Fire)

The game in fact contains three DLI routines; the first one changes the color
palette and changes the character set, and then chains to a second
DLI routine (by altering the Display List Interrupt Vector, `VDSLST`).
The second DLI routine sets the character set to the one containing the top half
of the tile set, and also alters one of the color registers every few scan lines,
to create the "Atari Rainbow" effect for the fire and gas leaks.  It then chains
to a third DLI routine, which is nearly identical to the second one, but with
the bottom half of the tile set.  It chains back to the second DLI routine.

### Animation and Color Palette

In reality, I have two sets of tile shapes, and flip between them every
few frames (screen refreshes) to get some animation -- fire burning,
gas leaks and water spraying, and workers flailng their limbs.
(See the files and documentation in the [`fonts/`](fonts/) subdirectory.)

The font animation is handled during a
[Vertical Blank Interurpt (VBI)](https://www.atariarchives.org/search.php?q=vertical+blank+interrupt)
routine, meaning any game loop slowdown that may happen will have no effect on
the animation frame rate.  It does this by setting some bytes (in Page 6 memory;
see [`page6.h`](src/page6.h)) which the DLI routines use to decide which character
sets to display.  It also resets the DLI Vector to point to the first DLI.

Another byte in Page 6 is used to let the DLI change which colors are used
in game area, based on events (e.g., flashing the background when an oil barrel
explodes, or greying out all colors while the game is paused).
(See [`dli.c`](src/dli.c).  Note: This is among the only hand-coded
6502 assembly language in this game.  Almost everything else is in straight C.)

Note: Currently, no Player/Missile Graphics ("sprites") are used in the game.
(They *are* used in the splash screen, though; see below.)

### Screen Shake

When an explosion is occurring, a countdown timer is used to handle
both the screen background flash (described above), explosion sound
effect volume (see below), and to enable a screen shake effect.

ANTIC provides various Display List instructions that will create between 1 and
8 blank scanlines.  The shaking is handled by simply setting changing one
line of the Display List (in the screen border area above the top of
the game area) to a random blank-scanline(s) instruction.  (That is, the
Atari's hardware vertical fine scrolling functionality is _not_ used.)

## Sound

### Title screen

The title screen has a timer that increments each frame.  During some
parts of its count, it will play an oscillating siren tone that
increases in volume (and pitch), then decreases in volume (and pitch).
(Yes, I made a vague attemt at a "firetruck speeding by"
[Doppler effect](https://en.wikipedia.org/wiki/Doppler_effect).)

Easter egg: If you push either stick in any direction on the title
screen, you can cause a firetruck horn blaring noise while the sirens
are audible!

### General Game Sounds

Various game sounds are played on one of the Atari's
[POKEY chip](https://en.wikipedia.org/wiki/POKEY)'s four "voices"
(channels), and is handled by a number of variables used to set the
pitch, initial volume, pitch change (if any), and speed of volume
decrease.  The pitch change (if applicable) and volume change occur
once per game loop.  (Note: Currently, they are *not* part of the VBI
routine.  Typically, games place their sound and music routines within
the VBI.)

This system is used for the majority of events that cause
sound effects.  They are:

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
automaton routine; see below.  That routine does the work of setting the
fire channel's sound registers.)

### Explosion Sound

This is a totally random noise sound played on its own dedicated
"voice", controlled by a countdown timer that affects its volume.
When something explodes, the timer is (re)set to its maximum,
and hence highest volume.

### Footsteps

The footstep sound is handled separately from the POKEY
chip voices, and is instead generated by using the
[GTIA chip](https://en.wikipedia.org/wiki/CTIA_and_GTIA)'s `CONSOL`
register, which basically acts as a 1-bit sound register.
(It's what the Atari uses to produce keyclick sounds when you type
on the keyboard, and the `[Control]`+`[2]` (`CHR$(125)`) buzzer
sound effect.)

On an Atari 400 or 800, this sound is produced by an internal speaker.
In the XL and XE models, it is combined with the POKEY's output
and goes to your TV/monitor's speaker.

## Gameplay

### Cellular Automaton

The game is driven by a
[cellular Automaton](https://en.wikipedia.org/wiki/Cellular_automaton)
routine.  The screen memory of the game area is treated as individual
cells.  Certain cell contents have behaviors which can affect their
own cell and/or the cells next to them.

#### Fire

A cell that contains a small fire has a random chance of growing into
a medium fire.  Similarly, a medium fire has a chance to become a
large fire.  A cell containing a large fire has a random chance of
spreading (creating a new small fire) in a random direction.

When spreading, if there's a worker (see below), they will perish.
If there's a gas leak or oil barrel, the spreading fire will will cause
an explosion (creating up to 4 large fires surrounding the destroyed object).

#### Workers

The workers are also treated as cells.  They "move" by placing a blank
shape in their current cell, and a worker in an adjacent cell.
They first observe nearby adjacent cells to see whether the firefighter
is nearby.  If so, they will go that direction; otherwise, they will
pick a random direction (possibly staying put).

#### Gas Leaks

Whenever broken pipes are observed, a counter is checked that
keeps track of how many valves are open on the screen.  If one or
more is open, then gas will leak in any blank spots adjacent to
broken pipes.  If no valves are open, any gas leaks will be removed.

#### Water

During the cellular automaton phase, any water on the level is
erased.  This means the main loop only needs to worry about
drawing new water if the player is pushing the spray joystick
(or `[Fire]` plus the main joystick).

### Water Spray

The player may spray water in any of eight directions.
Each direction is made up of four different tile shapes in the
character set (so we're using half of the game tile portion of
our character set just for water spray!  It *is* an important
aspect of the game, however!)

The sprays consist of a narrow point placed directly adjacent
to the firefighter, plus three more that represent the wide end
of the spray.  For the four cardinal directions, the wide spray
is drawn as three vertical or three horizontal shapes in a row.
For the four diagonal directions, the middle of the wide part
continues in the direction of spray, and the two ends appear
perpendicular to it (forming a 2×2 square of shapes).

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

When water spray hits fire, it reduces the fire's intensity
(large to medium to small) over time.

## Levels

### Level Creation

Levels are created as plain ASCII text files containing special
(human-readable) characters that represent the handful of various
shapes possible in the game.

See the [levels documentation](levels/README.md) for details.

The script [`level_to_dat.php`](tools/level_to_dat.php),
written in [PHP](https://en.wikipedia.org/wiki/PHP), reads the
individual ASCII text files in the [`levels/`](levels/) directory and
generates a binary file containing the game's internal representation
(aka the screen character bytes) of all of the levels.  (A single byte
at the beginning indicates how many levels there are, and two bytes
at the end of each level's data indicates where the firefighter's
starting position is to be.)

### Level Compression

The shapes that appear on the screen also include color data in their
highest two bits.  However, this game never use the same shape in different
colors; e.g., pipes are always green, brick walls and boxes are always red, etc.

Also, while we have 64 shapes in our character set to work with
(in up to 4 colors each) in reality many shapes are not part of the
level map.  (Half of the 64 characters in the set are used for the
various directions of water spray shapes!)

Therefore, we can strip the highest *three* bits of the level data,
and use those bits to indicate how many times the characters
repeat (once, twice, three times, four times, etc.).  Since we often
have the same character appear many times in a row (e.g., a row of
bricks, or a large blank area in a room), this allows us to store
one byte in the place of multiple bytes for the same level layout.
This is a form of
[run-length encoding](https://en.wikipedia.org/wiki/Run-length_encoding)
(RLE).

The script [`level_compress.php`](tools/level_compress.php) reads
the binary level data file (created by the tool above), which in
early beta versions was used directly (and level data in the game
executable was simply copied directly to screen memory) and
creates a compressed file.

As before, a single byte at the beginning indicates how many levels
there are.  That is followed by four-byte sequences for each level,
indicating the firefighter's starting position, and the offset within
the compressed level data that each level starts -- they are of course
different lengths depending on the compression!  Finally, all of the
compressed level data follows.

## High Score

The game records a top score, and allows the user to enter their
initials at the end of the game.  The disk-based version (see
the "Builds" section, below) provides a top-10 high score table,
which persists between sessions, as it is saved-to and loaded-from
the disk.

Code exists (in [`score.c`](src/score.c)) to use standard C file
input/output (I/O) functions -- `fopen()`, `fread()`, etc. --
to write to and read from a simple high score file, "`highscor.dat`".

However, by default the game stores the high score table directly
onto a pre-defined sector of the disk.  This means there's no "file"
attached to the data.  However, this allows one to easily extract the
data from the disk image; you always look at the same sector --
in this case 720 -- and don't need to worry about it moving around
between releases and dealing with the filesystem (the
[Volume Table of Contents (VTOC)](https://www.atariarchives.org/iad/chapter2.php))

Why bother? So that the high score table may be "shared" by multiple
players over the Internet! If the game disk image is made available to
users who use a [FujiNet](https://fujinet.online/) device, the
TNFS server hosting it can be told to grant players write-access
to *just* the sector containing the high score table (the rest
of the disk image will be read-only).

This feature of the TNFS server is implemented by using three
otherwise-unused bytes in the ATR disk image file format's header.
The script [`high_score_atr.php`](tools/high_score_atr.php) handles
this modification.  (See the FujiNet project's
["High Score storage for Legacy Games" page](https://github.com/FujiNetWIFI/fujinet-platformio/wiki/High-Score-storage-for-Legacy-Games)
for further information.)

A non-disk-based stand-alone build, with additional functionality
for FujiNet owners, stores a single high score directly onto an
SD storage card within a FujiNet (see "Builds", below).

## Help Screen

The disk version offers a help screen (press `[?]` or `[Help]`
on the title screen).  This contains exactly the same content
as [`README.md`](README.md), but it has been converted from
[Markdown](https://en.wikipedia.org/wiki/Markdown) to 40 column plain
ATASCII text.

### Markdown to ATASCII

This is done using the [`markdown`](https://daringfireball.net/projects/markdown/)
tool to convert to HTML, then the [`w3m`](https://w3m.sourceforge.net/)
text-based web browser, in `-dump` mode, to convert to 40-column text.

Text marked as code, using the "\`" (backtick) symbol, appears as
inverse video text on the Atari.  This is done by first pre-processing
`README.md` (using the [`tr`](https://www.gnu.org/software/coreutils/tr) tool)
to convert all backticks to an otherwise unused symbol, "~" (tilde).
That prevents the code from appearing as `<code>...</code>` in the resulting
HTML, and instead as `~...~`.  That appears in the final ASCII text file
that `w3m` generates as well, which is then processed by the small
[`tools/txt2atascii.php`](tools/txt2atascii.php) which both converts
Unix end-of-line (EOL) characters to ATASCII, and manages inverse video.

### Paging Backwards

Although the help text is read from a file on disk, "Firefighter"
allows you to paginate backwards; that is, navigate to previous pages.
The "note" and "point" DOS (disk operating system) commands are utilized
to make this possible.  As each new page is read, the starting location
of that part of the text is recorded in an array, via "note".  If you
want to go to a previous page, the "point" operation is used to seek
to that part of the file, and that page (screen-full) of text is re-read.

## Splash Screen

### Splash Graphics

The splash screen (disk version) is comprised of a pair of public
domain images, and text rendered in two fonts released under the SIL
Open Font License.  They can be found in the [`img-src` folder](img-src/).

They are assembled (composited), text is rendered, and they are scaled
down in both size and color palette (remapped) using
[ImageMagick](https://imagemagick.org/).

The resulting file, a [NetPBM](https://netpbm.sourceforge.net/)
Portable Gray Map (PGM), is converted to binary data suitable
for display in Atari GTIA "`GRAPHICS 9`" 16-greyscale mode (80×192 pixels)
by the script [`pgm2gr9.php`](tools/pgm2gr9.php).

Additionally, the darkest pixels from the image are extracted
to be used as the firefighter's silhouette, and stored in a
binary data file.  It is used to create a set of Player/Missile
Graphics (PMG) objects (aka "sprites") that appear on the left side
of the screen, and thus adding a 17th color to the splash screen.

A tiny program acts as the splash screen, loading and displaying
this picture.  I take advantage of _MyDOS_'s ability to chain
multiple "autorun" programs (aka `AUTORUN.SYS` in Atari DOS) by
having it load the splash first (`SPLASH.AR0`), followed by
the game itself second (`FIREFITE.AR1`).

### Splash Music

_Raster Music Tracker_ (http://raster.atariportal.cz/rmt/rmt.htm)
is a popular music tracker system for the Atari, originally created by
Radek Sterba (RASTER/C.P.U.) between 2003 and his untimely death in 2011.
Since then, others (e.g., VinsCool: https://github.com/VinsCool/RASTER-Music-Tracker
and Gianluca GP Renzi: https://github.com/gianlucarenzi/RITMO-Music-Tracker/)
have continued development.

_RITMO_ is a cross-platform, _Qt_-based fork, meaning I can use it with
ease on my Linux development system.

It offers a way of exporting an song module (`.rmt`) directly to
assembly language (`.asm`), which can be assembled into an object file and
linked to an executable program.

Starting with version 2.8, it can export code suitable for _CC65_'s `ca65` assembler.
(It also supports _Atasm_ and _Xasm_ style -- including _MADS_ -- assemblers.)
The _RITMO_ source code repository includes 6502 assembly code for an RMT playback
routine that runs on the Atari, and _RITMO_'s manaul includes instructions on how
to build it and combine it with a program written in C, along with examples.

#### Calling the player from C

The splash screen source code ([`splash.c`](src/splash.c)) itself
only required the following additions:

 * `#include "rmt.h"` -- Include the RMT C wrapper header, from _RITMO_
 * `extern const unsigned char rmt_module[];` -- Access to the song data
   (built from [`tune.asm`](music/tune.asm))
 * `rmt_init(rmt_module);` -- Initialize the player
 * `rmt_vbi_on();` to start music, `rmt_vbi_off();` to stop it
 * With `RMT_GLOBALVOLUMEFADE` feature enabled, it's possible to fade out music
   (and any sound effects), by setting `rmt_global_volume_fade` to a value
   between 0 (no fade) and 255 (silence) (technically 240, aka `$F0` hex).
   I do this in a loop while adjusting the color palette as a transition from
   the splash screen to a simple "Loading..." message.
   + To reduce DMA (direct memory access) by the ANTIC chip while loading,
     the "Loading..." screen is mostly assembled of blank scanlines,
     with a single `GRAPHICS 0` text line in the center.  The playfield
     is also narrowed.

#### Building the player, the music, and the splash executable:

Along with the minor source code additions described above,
_Firefighter_'s [`Makefile`](Makefile) was updated to build the music
player and data into the splash screen executable:

##### Build the song data as an object (`music/tune.asm` → `obj/tune.o`):

Simply assemble it:

> ```
ca65 -t atari music/tune.asm -o obj/tune.o
```

##### Build the RMT player, and grab its exported symbols

Use a shell script copied from the _RITMO_ source code repository
(version 2.8), [`obx.sh`](src/3rdparty/ritmo/ca65/obx.sh) -- along with a
linker configuration file (see below),
[`obx.cfg`](src/3rdparty/ritmo/ca65/obx.cfg), and an include file,
[`mads.inc`](src/3rdparty/ritmo/ca65/mads.inc) -- that invokes _CC65_'s
`ca65` and `ld65` to assemble and link the RMT player source.

The player source was also copied from _RITMO_'s version 2.8 repo -- in
this case, the [`Patch16` version](src/3rdparty/ritmo/ca65/Patch-16/).
(For info on the different versions, see
https://github.com/gianlucarenzi/RITMO-Music-Tracker/blob/master/asm/README.md)

The `obx.sh` script creates Atari binary file using _CC65_'s tools, but built
the same way _MADS_ assembler does:

> "assemble a ca65 source of asm/ca65 into an Atari binary file as MADS
> does: two passes of ca65 (the first finds the empty segments, see mads.inc),
> then ld65 with obx.cfg." -- _Comments at the top of `obx.sh`_

The `Makefile` invokes it like so:

> ```
OBX_SYMBOLS="RASTERMUSICTRACKER RMTGLOBALVOLUMEFADE" \
    src/3rdparty/ritmo/ca65/obx.sh \
        obj/player.obx \
        src/3rdparty/ritmo/ca65/Patch-16/rmtplayr.s \
        -D FEAT_IS_TRACKER=0 \
        -D FEAT_IS_SIMPLEP=1 \
        -D EXPORTXEX=0 \
        -D EXPORTSAP=0 \
        -D TABLES=\$3A00 \
        -D STEREOMODE=0 \
        -D FEAT_GLOBALVOLUMEFADE=1 \
    > obj/player.sym
```

The various `-D` symbol definitions specify which RMT features should
be made available, among other things.  (`FEAT_GLOBALVOLUMEFADE=1` is
what provides us with the `rmt_global_volume_fade` volume control variable,
and the functionality itself; it's optional, and not included in the player
if not requested.)

Along with the binary object `player.obx`, the above command also
captures the requested symbols (as specified by the `OBX_SYMBOLS` environment
variable) into the file `player.sym`.  In our case, that file ends up
containing:

> ```
OBX_SYMBOL RASTERMUSICTRACKER 3100
OBX_SYMBOL RMTGLOBALVOLUMEFADE 3209
```

The `Makefile` then uses the `awk` tool to convert those symbols and
addresses into a file that will be `.include`-ed by the _CC65_ wrapper
around the RMT player, [`cc65/rmt.s`](src/3rdparty/ritmo/cc65/rmt.s):

```
awk '$1 == "OBX_SYMBOL" && $2 == "RASTERMUSICTRACKER" \
     { print "RMT_PLAYER = $" $3 }' \
     obj/player.sym \
  > src/player.inc
```

```
awk '$1 == "OBX_SYMBOL" && $2 == "RMTGLOBALVOLUMEFADE" \
     { print "RMT_GLOBALVOLUMEFADE = $" $3 }' \
     obj/player.sym \
  >> src/player.inc
```

That results in a `player.inc` containing:

> ```
RMT_PLAYER = $3100
RMT_GLOBALVOLUMEFADE = $3209
```

##### Build the _CC65_ wrapper around the RMT player

Now that `player.inc` has been created, the C wrapper can be built:

> ```
ca65 -t atari -I src/ src/3rdparty/ritmo/cc65/rmt.s -o obj/rmt.o
```

##### Update the splash screen's linker configuration

The `ld65` linker requires a [configuration file](https://cc65.github.io/doc/ld65.html#s5)
that describes the memory layout, and assingment of 'segments' to memory areas.
(Font and level data, which are "baked into the game" are described in the main game
executable's config, [`atari.cfg`](src/atari.cfg).)

Since the splash screen program will now have music data and a player routine,
these things needs to be specified in its config:
[`atari-splash.cfg`](src/atari-splash.cfg).

I started with _RITMO_'s example config file for a simple RMT music player.
Importantly, it includes the following entries to get the player object (see above)
linked:

> ```
...
MEMORY {
  ...
  TRAILER:    file = %O,               start = $0000, size = $0006;
}
SEGMENTS {
  ...
  AUTOSTRT:  load = TRAILER,    type = ro;
}
...
```

I then received amazing help from Gianluca to get things working correctly, in
terms of where in memory to put things.  (The splash screen requires around
10KB of memory into which the bitmap image and Player/Missle Graphics are
currently loaded from disk -- they are not "baked in".  The splash program
itself and the music player are each about 4KB.  This all needs to fit
nicely within 48K, with _MyDOS_ resident.)

##### Declare the song data segment

Since the C code needs to know about the music data segment, declare
it in [`segments-splash.s`](src/segments-splash.s):

> ```
...
   ⇥    .export    ⇥    _rmt_module
.import    ⇥    RMT_SONG_DATA
_rmt_module = RMT_SONG_DATA
```

(As seen above, it's referenced in the C code as
"`extern const unsigned char rmt_module[];`".  Note: tabs in that
"`.s`" file are denoted in this document with spaces around a "⇥" symbol.)

##### Link everything together into the splash executable

First, build the splash screen, RMT player C wrapper, and music data
into an executable (`.xex`).  This is very similar to what it looked
like before music was added, and how I build other parts of the game:

Compile the C source into assembly:

> ```
cc65 ... -t atari src/splash.c -o asm/splash.s
```

Assemble the assembly code into objects:
 
> ```
ca65 ... -t atari asm/splash.s -o obj/splash.o
ca65 ... -t atari asm/segments-splash.s -o obj/segments-splash.o
```

Link the objects together into an executable:

> ```
ld65 ... \
  -o obj/splash1.xex \
  -C src/atari-splash.cfg \
  obj/splash.o obj/segments-splash.o obj/rmt.o obj/tune.o atari.lib
```

The objects being linked above are:

 * `splash.o` -- the main C code to the splash screen program, compiled and assembled
 * `segments-splash.o` -- the splash screen's segments (see above),
 * `rmt.o` -- the C wrapper around the RMT player, compiled and assembled
 * `tune.o` -- the song, as exported to a `ca65`-style assembly language file, assembled

Finally, concatenate the splash screen and the RMT player itself
into the final executable, using a simple `cat` command:

> ```
cat obj/splash1.xex obj/player.obx > splash.xex
```

Note: This is my first foray into using a real music player in a
project.  This is why I have (for now, at least) restricted it to the
simple splash screen of this game.

Within the Atari game development community, use of C seems
much less common than straight assembly,
[_MAD Pascal_](https://github.com/tebe6502/Mad-Pascal), or even
[_FastBasic_](https://github.com/dmsc/fastbasic).

This is why I have attempted to document above the steps taken to
get it working -- both for others, as well as for my future self.

I received a tremendous amount of support, including patches,
from _RITMO_'s developer, [Gianluca Renzi](https://github.com/gianlucarenzi)),
and for that I am extremely grateful.

## Builds

There are three builds that the [`Makefile`](Makefile) can create
(its primary "`all`" target builds all three by default):

 * `firefite.atr` -- A disk image containing a copy of
   [_MyDOS_](https://www.mathyvannisselroy.nl/mydos.htm),
   the splash screen program and data (graphics and music),
   the main game program, the help text file, and a high score table
   file.  The game program is built to support the top-10 high score
   table, and read the help text.  It is constructed using the
   `dir2atr` tool from HiassofT's
   [_AtariSIO driver and utilities_ collection](https://github.com/HiassofT/AtariSIO)
 * `firefite.xex` -- A stand-alone executable version of the game,
   with no splash, no help screen, and no high score table;
   it records a single high score in memory.
 * `fireftfn.xex` -- Identical to the stand-alone version, but which uses
   [FujiNet AppKey storage](https://github.com/FujiNetWIFI/fujinet-firmware/wiki/SIO-Command-%24DC-Open-App-Key)
   to persistently record a single high score, as well as game configuration
   (joystick preferences), to the FujiNet itself.

