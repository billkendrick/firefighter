#!/usr/bin/php
<?php
/*
  Add high-score capability to header of ATR disk image for Firefighter

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-09-04 - 2023-09-04
*/

$fi = fopen($argv[1], "rb");
if ($fi == NULL) {
  fprintf(STDERR, "Cannot open input '%s'\n", $argv[1]);
  exit(1);
}

$fo = fopen($argv[2], "wb");
if ($fo == NULL) {
  fprintf(STDERR, "Cannot open output '%s'\n", $argv[2]);
  exit(1);
}

/* Read & write the first part of the header */
for ($i = 0; $i < 12; $i++) {
  $c = ord(fgetc($fi));
  fputs($fo, chr($c), 1);
}

/* Replace relevant bytes of the header to allow high-score support */
fgetc($fi);
fgetc($fi);
fgetc($fi);

fputs($fo, chr(1)); /* One high-score enabled sector */
fputs($fo, chr(208)); /* Low-byte of "720" */
fputs($fo, chr(2)); /* High-byte of "720" */

/* Read & write the rest of the file */
while (!feof($fi)) {
  $c = ord(fgetc($fi));
  if (!feof($fi)) {
    fputs($fo, chr($c), 1);
  }
}

fclose($fi);
fclose($fo);

