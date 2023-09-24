/*
  Firefighter FujiNet App Key support

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-09-24 - 2023-09-24
*/

#include <atari.h>
#include "app_key.h"
#include "sio.h"

#define APP_KEY_SIO_DEVICEID 0x70 /* FujiNet App Key */
#define APP_KEY_SIO_UNIT 1
#define APP_KEY_SIO_TIMEOUT 1 /* very quick, because we want to move on with our lives */

#define APP_KEY_SIO_CMD_OPEN 0xDC
#define APP_KEY_SIO_CMD_APPKEY_READ 0xDD
#define APP_KEY_SIO_CMD_APPKEY_WRITE 0xDE

/* (From https://github.com/FujiNetWIFI/fujinet-apps/blob/master/appkey-sample/src/main.c
   by Oscar Fowler) */
typedef union {
  struct
  {
    unsigned int creator;
    unsigned char app;
    unsigned char key;
    unsigned char mode;
    unsigned char reserved;
  } open;
  struct
  {
    unsigned int length;
    unsigned char value[MAX_APPKEY_LEN];
  } read;
  struct
  {
    unsigned char value[MAX_APPKEY_LEN];
  } write;
} datablock;

static datablock app_key_data;

#define APPKEY_OPEN_READ 0
#define APPKEY_OPEN_WRITE 1


/* The data we'll be reading/writing */
extern char level;
extern char main_stick;
extern unsigned long int high_score;
extern char high_score_name[4]; 

/**
 * Open our app key store on the FujiNet
 * (used by the read & write routines, below).
 *
 * @param byte mode - 0=read, 1=write
 * @return byte SIO status
 */
unsigned char open_appkey(unsigned char mode) {
  PIA.pactl = PIA.pactl | 1; /* Seen in FN_Open() in bocianu's fn_sio.pas */
  app_key_data.open.creator = APPKEY_CREATOR_ID;
  app_key_data.open.app = APPKEY_APP_ID;
  app_key_data.open.key = 0x00; /* Our one and only Key ID */
  app_key_data.open.mode = mode;
  app_key_data.open.reserved = 0x00;

  OS.dcb.ddevic = APP_KEY_SIO_DEVICEID;
  OS.dcb.dunit = APP_KEY_SIO_UNIT;
  OS.dcb.dtimlo = APP_KEY_SIO_TIMEOUT;

  OS.dcb.dcomnd = APP_KEY_SIO_CMD_OPEN;
  OS.dcb.dstats = SIO_WRITE;
  OS.dcb.dbuf = &app_key_data;
  OS.dcb.dbyt = sizeof(app_key_data.open);
  OS.dcb.daux = 0;

  siov();

  return OS.dcb.dstats;
}

/**
 * Read FireFighter data from FujiNet app key store.
 *
 * Note: Fails silently.
 */
void read_settings(void) {
  unsigned char dstats;

  dstats = open_appkey(APPKEY_OPEN_READ);
  if (dstats == 1 /* operation complete (no errors) */) {
    OS.dcb.ddevic = APP_KEY_SIO_DEVICEID;
    OS.dcb.dunit = APP_KEY_SIO_UNIT;
    OS.dcb.dtimlo = APP_KEY_SIO_TIMEOUT;

    OS.dcb.dcomnd = APP_KEY_SIO_CMD_APPKEY_READ;
    OS.dcb.dstats = SIO_READ;
    OS.dcb.dbuf = &app_key_data;
    OS.dcb.dbyt = sizeof(app_key_data.read);
    OS.dcb.daux = 0;

    siov();

    if (OS.dcb.dstats == 1 /* operation complete (no errors) */) {
      high_score = (
        ((unsigned long int) app_key_data.read.value[0] << 24) |
        ((unsigned long int) app_key_data.read.value[1] << 16) |
        ((unsigned long int) app_key_data.read.value[2] << 8) |
        ((unsigned long int) app_key_data.read.value[3])
      );

      high_score_name[0] = app_key_data.read.value[4];
      high_score_name[1] = app_key_data.read.value[5];
      high_score_name[2] = app_key_data.read.value[6];
      high_score_name[3] = '\0';

      main_stick = app_key_data.read.value[7];
      level = app_key_data.read.value[8];

      /* Per https://github.com/FujiNetWIFI/fujinet-platformio/wiki/SIO-Command-$DB-Close-App-Key,
         "[The "Close App Key"] command is currently not needed, but
         exists for possible future use. Both the Write App Key and
         Read App Key will immediately close the key after the command
         is executed."
      */
    }
  }
}

/**
 * Write FireFighter data to FujiNet app key store.
 *
 * @return bool whether it seemed to succeed
 */
unsigned char write_settings(void) {
  unsigned char dstats;

  dstats = open_appkey(APPKEY_OPEN_WRITE);
  if (dstats == 1 /* operation complete (no errors) */) {
    app_key_data.write.value[0] = (high_score & 0xFF000000) >> 24;
    app_key_data.write.value[1] = (high_score & 0x00FF0000) >> 16;
    app_key_data.write.value[2] = (high_score & 0x0000FF00) >> 8;
    app_key_data.write.value[3] = (high_score & 0x000000FF);

    app_key_data.write.value[4] = high_score_name[0];
    app_key_data.write.value[5] = high_score_name[1];
    app_key_data.write.value[6] = high_score_name[2];

    app_key_data.write.value[7] = main_stick;
    app_key_data.write.value[8] = level;

    OS.dcb.ddevic = APP_KEY_SIO_DEVICEID;
    OS.dcb.dunit = APP_KEY_SIO_UNIT;
    OS.dcb.dtimlo = APP_KEY_SIO_TIMEOUT;

    OS.dcb.dcomnd = APP_KEY_SIO_CMD_APPKEY_WRITE;
    OS.dcb.dstats = SIO_WRITE;
    OS.dcb.dbuf = &app_key_data.write;
    OS.dcb.dbyt = sizeof(app_key_data.write);
    OS.dcb.daux = 9; /* How many bytes of data (see above) */

    siov();

    /* Per https://github.com/FujiNetWIFI/fujinet-platformio/wiki/SIO-Command-$DB-Close-App-Key,
       "[The "Close App Key"] command is currently not needed, but
       exists for possible future use. Both the Write App Key and
       Read App Key will immediately close the key after the command
       is executed."
    */

    if (OS.dcb.dstats == 1 /* operation complete (no errors) */) {
      return 1;
    }
  }
  /* Failed to open, or failed to write */
  return 0;
}
