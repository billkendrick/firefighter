#!/usr/bin/php
<?php
/*
  Compress level data for Firefighter

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-26 - 2023-08-26

  Compresses level data by stripping two high bits
  (used for color in ANTIC Mode 7 (GRAPHICS 2),
  so we can "infer" them when we load the level),
  and then using those two high bits to represent
  how many times a shape repeats (up to four times).

  FIXME: WIP
*/

require_once("tools/level_consts.inc.php");

$INFILE = "data/levels.dat";
$OUTFILE = "data/levels_cmp.dat";


/* READ INPUT FILE: */
/* ================ */

$fi = fopen($INFILE, "rb");
if ($fi == NULL) {
  fprintf(STDERR, "Cannot open '%s'\n", $INFILE);
  exit(1);
}


$level_cnt = ord(fgetc($fi));
printf("There are %d levels.\n\n", $level_cnt);

$stream = array();
$ffx = array();
$ffy = array();

for ($l = 0; $l < $level_cnt; $l++) {
  printf("Level %d:\n", $l + 1);

  for ($y = 0; $y < $LEVEL_H; $y++) {
    for ($x = 0; $x < $LEVEL_W; $x++) {
      $c = ord(fgetc($fi)) & bindec("00111111");
      $stream[] = $c;
    }
  }

  $ffx[$l] = ord(fgetc($fi));
  $ffy[$l] = ord(fgetc($fi));
  printf("Firefighter @ (%d,%d)\n", $ffx[$l], $ffy[$l]);

  echo "\n";
}

fclose($fi);


/* COMPRESS LEVELS: */
/* ================ */

$data = array();
$offset = 0;
$level_offsets = array();

for ($l = 0; $l < $level_cnt; $l++) {
  $level_offsets[$l] = $offset;

  for ($i = 0; $i < ($LEVEL_H * $LEVEL_W); $i++) {
    $data[$offset++] = $stream[$i];
  }
}


/* WRITE OUTPUT FILE: */
/* ================== */

$fo = fopen($OUTFILE, "wb");
if ($fo == NULL) {
  fprintf(STDERR, "Cannot open '%s'\n", $OUTFILE);
  exit(1);
}

/* HEADER: */
/* ------- */

/* First byte: How many levels there are */
fputs($fo, chr($level_cnt), 1);

for ($l = 0; $l < $level_cnt; $l++) {
  /* First two bytes of each level, in the header:
     Firefighter's starting X & Y position: */
  fputs($fo, chr($ffx[$l]), 1);
  fputs($fo, chr($ffy[$l]), 1);

  /* Next two bytes of each level, in the header:
     Compressed level data's offset:
     (high byte first; big endian) */
  fputs($fo, chr(($level_offsets[$l] & 0xFF00) >> 8), 1);
  fputs($fo, chr($level_offsets[$l] & 0x00FF), 1);
}

/* LEVEL DATA: */
/* ----------- */

for ($i = 0; $i < $offset; $i++) {
  fputs($fo, chr($data[$i]), 1);
}

fclose($fo);

printf("%d levels (%d raw bytes) compressed to %d bytes\n",
  $level_cnt, ($level_cnt * $LEVEL_H * $LEVEL_W), $offset);

