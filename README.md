# Firefighter

Alpha

## Credits
By Bill Kendrick <bill@newbreedsoftware.com>

Inspired by _Real Heroes: Firefighter_ (Wii, Conspiracy Entertainment, 2012)
_Brave Firefighters_ (SEGA, arcade, 1999), and _Robotron: 2084_
(Williams Electronics, arcade, 1982).

Developed 2023-08-13 - 2023-08-19

------------------------------------------------------------------------

# Story

Fires are breaking out at an industrial building.  It's your job to find
and save the workers trapped inside, and put out fires along the way.

------------------------------------------------------------------------

# Gameplay

## Overview

Each level of the game is made of a single screen containing one or more
rooms to navigate, occupied by crates, barrels of explosive material,
leaking gas pipelines, and workers.

You control a firefighter with an unlimited amount of fire extinguishing
agent (a magic invisible hose? a decanter of endless water? don't think
about it too hard).  Spray the fire to quench it, but beware because it
spreads quickly.

Find the workers trapped in the room, and help them navigate to the exit.
They are overcome by smoke and fear, so wander randomly unless you are
close by to guide them.  They can perish if they end up too close to a
large fire!

Some rooms contain crates and/or barrels which you can push out of
your way if they block your path.  Be careful how you move them, though;
you can only push, you cannot pull them!

Pipes pump gas through some rooms, and any cracks leak this flammable
material into the air.  If you can reach the valve, you can turn off
the flow.

If there's an ax, you can pick it up.  Instead of pushing crates, you
will destroy them (they may take two hits).  Be careful navigating near
pipes once you have the ax, as you will damage them!

When fire spreads to oil barrels or gas leaking from pipes it will
cause an explosion, spreading even more fire.  Beware of chain reactions!

## Starting the Game

[TBD] DESCRIBE TITLE SCREEN CONTROLS AND STARTING THE GAME.

## Controls

This game is best played with a dual-stick set-up (a la the arcade games
_Robotron: 2084_ or _Smash TV_).  One stick controls your firefighter's
position in the room, the other causes you to spray; in both cases,
in any one of eight directions.

If you prefer to play with only one joystick, you can hold the fire button
and push a direction to spray that way.  However, you will stand still
while you spray!

## Items

[TBD] DESCRIBE EVERY ITEM.

## Completing a Level

[TBD] HOW TO END THE LEVEL.

## Scoring

[TBD] DESCRIBE SCORING.

------------------------------------------------------------------------

# Development

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
screen data.  This means only 64 shapes are available.

However, when setting the `CHBASE` register (or the OS's `CHBAS` shadow),
it's possible to offset the character set page by two, which causes the
ANTIC 7 parts of the screen to use the latter half of the character set.
The small text, ANTIC mode 2 (aka `GRAPHICS 0`) continues to use the full set.
Therefore, while I'm limited to only uppercase characters the small mode 2 text,
and no text at all (only game tile shapes) in the large mode 7 text, this means
I can get away with only a single 1KB character set for all my needs!

In reality, I have two character sets, and flip between them every few frames
(screen refreshes) to get some animation -- fire burning, gas leaks and
water spraying, and workers flailng their limbs.  So in the end, 2KB of space
is used for all of the character data.  (See the `fire1.fnt` and `fire2.fnt`
files in the `fonts/` subdirectory.)

You'll notice large text used for the "FIREFIGHTER" title at the top.
This is acheived by _not_ offsetting the character set at first; that happens
through a Display List Interrupt (which also changes some colors in the
palette, and therefore also handles the fire and gas leak color cycling, etc.),
and the font animation happens hear.  (So any game loop slowdown that may happen
has no effect on these things.)

One byte in Page 6 is used to tell the DLI where the font's base is (there may
be a better way of handling this?), and it uses `RTCLOK` (updated by the OS
each Vertical Blank Interrupt) to animate.  (I don't use any of my own VBIs.)

Another byte in Page 6 is used to let the DLI change the background color of
the game area, based on events (e.g., flashing when an oil barrel explodes).
(See `dli.c` in the `src/` subdirectory.  This is the only hand-coded 6502
assembly language in this game.)

## Sound

[TBD] Explain how sound works.

## Gameplay

[TBD] Explain how gameplay & cellular automata work.

## Levels

[TBD] Explain how levels are designed.

