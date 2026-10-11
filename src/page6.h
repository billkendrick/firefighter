/*
  Firefighter Page 6 struct and some color definitions

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  https://www.newbreedsoftware.com/firefighter/

  2026-10-11 - 2026-10-11
*/

struct page6 {
  unsigned char font; // chbase of animated tile shapes (flips every few frames)
  unsigned char color_bak; // background
  unsigned char color_fire; // fire hue
  unsigned char color_green; // pipes, etc.
  unsigned char color_blue; // water, etc.
  unsigned char fire_flicker;
  unsigned char anim;
  unsigned char font_top; // chbase of top half of tile shapes (swaps every other mode line)
  unsigned char font_bot; // chbase of bottom half of tile shapes (swaps every other mode line)
};

#define PAGE6 (*(struct page6*)0x600)

#define DEF_COLOR_FIRE 0x20 /* yellow hue */
#define DEF_COLOR_GREEN 0xCA /* medium green */
#define DEF_COLOR_BLUE 0x86 /* medium blue */

#define DEF_TITLE_TEXT 0x52 /* purplish-red */
#define DEF_COLOR_CYAN 0xA8 /* light cyan */
#define DEF_COLOR_GOLD 0x14 /* gold */
#define DEF_COLOR_GREY 0x02 /* dark grey */

#define DEF_COLOR_RED 0x46 /* red */
