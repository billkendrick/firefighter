/*
  Firefighter FujiNet App Key support

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-09-24 - 2023-09-24
*/

#ifndef APP_KEY_H
#define APP_KEY_H

#define MAX_APPKEY_LEN 64

/* Bill Kendrick's App Key "Creator ID"
   (see https://github.com/FujiNetWIFI/fujinet-platformio/wiki/SIO-Command-%24DC-Open-App-Key) */
#define APPKEY_CREATOR_ID 0xB177

/* "FireFighter" App ID */
#define APPKEY_APP_ID 0x03

/* N.B. We only record a few bytes of data, so only use one key (Key Id 0) */

void read_settings(void);
unsigned char write_settings(void);

#endif // APP_KEY_H
