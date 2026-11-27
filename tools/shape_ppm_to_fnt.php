#!/usr/bin/php
<?php
/*
  Convert a PPM pixmap image into a multicolor Atari character set font

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  https://www.newbreedsoftware.com/firefighter/

  Last updated: 2027-11-27
*/

if ($argc < 3) {
  fprintf(STDERR, "Usage: %s input-512x65px.ppm output.fnt\n", $argv[0]);
  exit(1);
}

$in_file = $argv[1];
$out_file = $argv[2];

$fi = fopen($in_file, "rb");
if ($fi == NULL) {
  fprintf(STDERR, "Cannot open input file \"%s\"\n", $in_file);
  exit(1);
}
$fo = @fopen($out_file, "wb");
if ($fo == NULL) {
  fprintf(STDERR, "Cannot open output file \"%s\"\n", $out_file);
  exit(1);
}

$str = fgets($fi);
if ($str != "P6\n") {
  fprintf(STDERR, "Expected 'P6', got %s", trim($str));
  exit(1);
}

/* Assume there's a one-line GIMP comment */
$str = fgets($fi);

$str = fgets($fi);
sscanf($str, "%d %d", $w, $h);
if ($w != 512 || $h != 65) {
  fprintf(STDERR, "Expected it to be 512x65, got %s", trim($str));
  exit(1);
}

$str = fgets($fi);
if ($str != "255\n") {
  fprintf(STDERR, "Expected '255' for maxval, got %s", trim($str));
  exit(1);
}


/* From the first row, the first 10 pixels represent the
   colors to be used in the picture (PBM pixels per Atari pixel).
   The 4th and 5th color will be mapped to the same `11` bit pair. */
$colors = array();
for ($x = 0; $x < 5; $x++) {
  $r = ord(fgetc($fi));
  $g = ord(fgetc($fi));
  $b = ord(fgetc($fi));
  $_ = ord(fgetc($fi));
  $_ = ord(fgetc($fi));
  $_ = ord(fgetc($fi));

  $colors[sprintf("%02x%02x%02x", $r, $g, $b)] = min($x, 3);
}


/* Skip the rest of the first row... */
for ($x = 5; $x < 256; $x++) {
  $r = ord(fgetc($fi));
  $g = ord(fgetc($fi));
  $b = ord(fgetc($fi));
  $_ = ord(fgetc($fi));
  $_ = ord(fgetc($fi));
  $_ = ord(fgetc($fi));
}


/* Read the remaining row to collect the pixels */
$font = array();
for ($y = 0; $y < 64; $y++) {
  $font[$y] = array();

  for ($x = 0; $x < 32; $x++) {
    /* Four pixels (of one character), left half of tile */
    $c1 = get_color($fi);
    $c2 = get_color($fi);
    $c3 = get_color($fi);
    $c4 = get_color($fi);

    $font[$y][$x * 2] = $c1 << 6 | $c2 << 4 | $c3 << 2 | $c4;

    /* Four pixels (of one character), right half of tile */
    $c1 = get_color($fi);
    $c2 = get_color($fi);
    $c3 = get_color($fi);
    $c4 = get_color($fi);

    $font[$y][$x * 2 + 1] = $c1 << 6 | $c2 << 4 | $c3 << 2 | $c4;
  }
}


/*
  Scan through the bitmap like so:

   + Rows 00 - 07 -- top half of first chunk of first frame
   + Rows 16 - 23 -- top half of second chunk of first frame
   + Rows 08 - 15 -- bottom half of first chunk of first frame
   + Rows 24 - 31 -- bottom half of second chunk of first frame

   + Rows 32 - 39 -- top half of first chunk of second frame
   + Rows 48 - 55 -- top half of second chunk of second frame
   + Rows 40 - 47 -- bottom half of first chunk of second frame
   + Rows 56 - 63 -- bottom half of second chunk of second frame
*/
for ($frame = 0; $frame < 2; $frame++) {
  for ($half = 0; $half < 2; $half++) {
    for ($chunk = 0; $chunk < 2; $chunk++) {
      $yy = ($frame * 32) + ($chunk * 16) + ($half * 8);

      for ($x = 0; $x < 64; $x++) {
        for ($y = 0; $y < 8; $y++) {
          $o = $font[$yy + $y][$x];
          fputs($fo, chr($o));
        }
      }
    }
  }
}

fclose($fi);
fclose($fo);


/* Read a pixel from the pixmap file, and look up which
   color (0, 1, 2, or 3) it represents & return that value. */
function get_color($fi) {
  global $colors;

  $r = ord(fgetc($fi));
  $g = ord(fgetc($fi));
  $b = ord(fgetc($fi));
  $_ = ord(fgetc($fi));
  $_ = ord(fgetc($fi));
  $_ = ord(fgetc($fi));

  $rgb = sprintf("%02x%02x%02x", $r, $g, $b);
  if (!array_key_exists($rgb, $colors)) {
    fprintf(STDERR, "Unrecognized RGB %s at %d,%d\n", $rgb, $x, $y);
    exit(1);
  }

  return ($colors[$rgb]);
}
