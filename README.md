# Firefighter

### Version: 0.1-BETA10

A twin-stick firefighting action game for the Atari 8-bit.

## Credits
> By Bill Kendrick <bill@newbreedsoftware.com>  
> http://www.newbreedsoftware.com/firefighter/
>
> Inspired by [_Real Heroes: Firefighter_](https://en.wikipedia.org/wiki/Real_Heroes:_Firefighter)
> (Wii; Conspiracy Entertainment, 2012),
> [_Brave Firefighters_](https://www.arcade-museum.com/game_detail.php?game_id=7212)
> (arcade; SEGA, 1999),
> [_Boulderdash_](https://en.wikipedia.org/wiki/Boulder_Dash_%28video_game%29)
> (Atari 8-bit and other platforms; First Star Software, 1985),
> and [_Robotron: 2084_](https://en.wikipedia.org/wiki/Robotron:_2084)
> (arcade; Williams Electronics, 1982).
> See the [development docs](DEVELOPMENT.md).
>
> Splash screen photo: "Firefighter: Fireman in Action"
> by Jean Beaufort (Public Domain).
>
> Splash screen firefighter badge: From "First Responders Three"
> by Byron Songer (Public Domain).
>
> Splash screen fonts: "Echotopia" by Erik Kirtley
> and "K.O. Activista" by deFharo (both SIL Open Font License).
>
> Level 7 by Edward

Developed 2023-08-13 - 2024-07-18

------------------------------------------------------------------------

# I. Story

> Fires are breaking out at an industrial building.  It's your job to find
> and save the workers trapped inside, and put out fires along the way.

------------------------------------------------------------------------

# II. Gameplay Overview

> Each level of the game is made of a single screen containing one or more
> rooms to navigate, occupied by crates, barrels of explosive material,
> leaking gas pipelines, and workers.
>
> You control a firefighter with an unlimited amount of fire extinguishing
> agent (a magic invisible hose? a decanter of endless water? don't think
> about it too hard).  Spray the fire to quench it, but beware because it
> spreads quickly.
>
> Find the workers trapped in the room, and help them navigate to the exit.
> They are overcome by smoke and fear, and will wander aimlessly unless you
> are close by to guide them.  They can perish if they end up too close to a
> large fire!
>
> Lose three (3) workers, and the game ends.
>
> Some rooms contain crates and/or barrels which you can push out of
> your way if they block your path.  Be careful how you move them, though;
> you can only push, you cannot pull them!  (You can also push workers if
> they get in your way.)
>
> Pipes pump gas through some rooms, and any cracks will leak this flammable
> material into the air.  If you can reach the valve, you can turn off
> the flow.
>
> If there's an ax, you can pick it up.  Instead of pushing crates, you
> will destroy them (they may take two hits).  Be careful navigating near
> pipes once you have the ax, as you will damage them!
>
> When fire spreads to oil barrels or gas leaking from pipes it will
> cause an explosion, spreading even more fire.  Beware of chain reactions!

------------------------------------------------------------------------

# III. Starting the Game

> The title screen shows some brief instructions and descriptions of
> objects.  Press the `Start` console key or firebutton on either
> joystick to begin a game.
>
> You can select which joystick moves the firefighter, and which sprays,
> by pressing the `Option` console key.
>
> You can choose a starting level by using the `Select` console key.

------------------------------------------------------------------------

# IV. Controls

> This game is best played with a dual-stick set-up (a la the arcade games
> _Robotron: 2084_ or _Smash TV_).  One stick controls your firefighter's
> position in the room, the other causes you to spray; in both cases,
> in any one of eight directions.  You may move and spray at the same time
> (walking backwards or strafing).
>
> If you prefer to play with only one joystick, you can hold the fire button
> and push a direction to spray that way.  However, you will stand still
> while you spray!
>
> Press `Start` or `Esc` to abort the game.  Use the joystick and
> firebutton, `Select` and `Start`, or `Y` or `N` keys to confirm or
> cancel aborting.
>
> Press `Space` or `P` to pause and unpause gameplay.

------------------------------------------------------------------------

# V. Object and Item Descriptions

> ## 1. Status Bar
>
> > Across the top of the screen you'll be shown:
> >
> > * "LEVEL" - The current level
> > * "SCORE" - Your score
> > * "BONUS" - The current level's bonus (which counts down)
> > * Fire icon - How many workers you have lost during this game
>
> ## 2. The Room
>
> > Neither you, nor the workers can move through these objects.
> > Fire cannot spread into or through these objects.
> >
> > ### A. Brick walls
> >
> > > The walls that make up the room(s) in each level.
> >
> > ### B. Exit Sign and Door
> >
> > > Walk up to these and push into them, and any workers standing next to
> > > you will leave the room to safety.  (This is the goal of the game!)
> > >
> > > Once all workers have left the room (or perished), you can push into
> > > a door or exit leave the room yourself, and thus end the level.
> > > (See "Completing a Level", below.)
> >
> > ### C. Gas Pipes and Valves
> >
> > > #### i. Gas Pipes
> > >
> > > > Gas pipes which are broken will leak gas, which cause explosions.
> > > > Large fire will appear at the gas leak's position, as well as
> > > > directly above, below, to the left, and to the right, if possible.
> > > >
> > > > In some levels, gas pipes start out broken.  Beware -- if you have the
> > > > ax and walk into a pipe, it will become broken!
> > >
> > > #### ii. Valves
> > >
> > > > Closing all valves connected to pipes will turn off the gas,
> > > > which will stop any broken pipes from leaking gas.
> > > >
> > > > Walk up and push into a valve to toggle its state.
> > > > Open valves have crossbars that go up and down ("+").
> > > > Closed valves have crossbars that go diagonal ("X").
>
> ## 3. Movable and Destroyable Objects
>
> > There are some objects which you may be able to push around the room.
> > Walk up and push into them any of eight directions and, if there is
> > nothing in that direction -- it must be a blank space, there may not
> > be fire, gas leaks, or workers -- it will move to that position
> > (you will stay in the same spot for a moment).
> >
> > ### A. Crates and Broken Crates
> >
> > > If you have picked up the ax during the current level, when you push
> > > into crates they will become broken crates.  And if you push into
> > > broken crates, they will be completely destroyed.
> > >
> > > When fire spreads to crates and broken crates, it starts out as
> > > a large fire immediately!
> >
> > ### B. Oil Barrels
> >
> > > When fire spreads to oil barrels, they cause an explosion!
> > > Large fire will appear at the barrel's position, as well as
> > > directly above, below, to the left, and to the right, if possible.
>
> ## 4. Workers
>
> > Workers are the green, flailing people running around the room.
> > They normally move randomly, but if you are nearby, they will
> > tend to move towards you.  Use this behavior to lead them to the
> > exit.  Stand next to the exit and push your joystick towards a
> > door or exit sign, and any adjacent workers will be rescued:
> > they disappear from the room, you'll hear a chime sound, and
> > you'll receive points.
> >
> > If a large fire ends up spreading onto a worker, they'll
> > perish: they disappear from the room, you'll hear a sad tone,
> > and the screen will flash red.
> >
> > Lose three (3) workers, and the game ends.
>
> ## 5. Ax
>
> > The ax is a collectible object that appear on some levels.
> > (You start each level without the ax.)  When you touch it, it
> > will disappear from the room, you'll hear a chime, you'll
> > receive some points, and the word "AX" will appear at the top right.
> > (Note: You cannot "unequip" yourself of the ax.)
> >
> > As described above, the ax can be used to break and destroy crates,
> > but can also break pipes if you're not careful -- and that can lead
> > to gas leaks, which lead to explosions, which leads to even more
> > fire to contend with!
> >
> > The ax can be destroyed by fire!  When fire spreads onto the ax,
> > it starts out as a large fire immediately!
>
> ## 6. Fire
>
> > Each level begins with, at the very least, a small fire somewhere
> > on the screen.  As the game goes on, small fires will grow into
> > medium fires, medium fires will grow into large fires, and large
> > fires will spread into adjacent positions in the room.
> >
> > Normally, when fire spreads, the new fire will start out small.
> > However, as described above, when fire spreads onto some objects,
> > the new fire will start out large.  In some cases, there'll be
> > an explosion (the screen will flash and an explosion sound will
> > play), which can cause more large fire to appear adjacent to
> > whatever caused the explosion!

------------------------------------------------------------------------

# VI. Completing a Level

> When no more workers remain on the screen -- either by being rescued,
> or perishing -- you may exit the level.  This is indicated by a blue
> exclamation point appearing at the top left of the screen.
>
> Walk to a door or "Exit" sign and hold the stick in the direction of
> the door or sign.  A countdown will appear at the top left.  Once it
> reaches zero (0), you will exit the level.
>
> Prior to leaving a level, you should try and put out any remaining
> fire and ensure there are no gas leaks, to receive a "safety bonus".
>
> Each level begins with a "Bonus" score that counts down over time.
> These points are also added to your score when you complete a level.

------------------------------------------------------------------------

# VII. Scoring

* Picking up an ax: 15 points
* Breaking or destroying a crate: -1 point
* Breaking a pipe: -5 points
* Saving a worker: 100 points
* Ending a level with no fire or gas leaks remaining: 1,000 points
* Time bonus: Scoring potental starts at 1,000 x Level # (max 10,000)

