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

#define DEF_TITLE_TEXT 0x52 /* color0 */
#define DEF_COLOR_1 0xCA
#define DEF_COLOR_2 0xA8 /* light cyan */
#define DEF_COLOR_3 0x46
#define DEF_COLOR_4 0x02

