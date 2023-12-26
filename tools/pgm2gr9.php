#!/usr/bin/php
<?php
/*
  Converts title screen from NetPBM portable greymap (PGM)
  to 16-shade GRAPHICS 9 image file for the Atari

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-09-19 - 2023-12-26
*/

$INFILE = "img-src/title.pgm";
$OUTFILE = "data/title.gr9";

/* Open input file */
$fi = fopen($INFILE, "r");
if ($fi == NULL) {
  fprintf(STDERR, "Cannot open '%s' for read!\n", $INFILE);
  exit(1);
}

/*
P2
80 192
255
*/

/* Get & check header */
$line = fgets($fi);
if (trim($line) != "P2") {
  fprintf(STDERR, "Input '%s' is not a 'P2' PGM file!\n", $INFILE);
  exit(1);
}

/* Eat any comments */
$pos = ftell($fi);
$line = fgets($fi);
while (substr($line, 0, 1) == '#') {
  $pos = ftell($fi);
  $line = fgets($fi);
}

fseek($fi, $pos);
list($w, $h) = fscanf($fi, "%d %d\n");
if ($w != 80 || $h != 192) {
  fprintf(STDERR, "Input '%s' is not 80x192; it's %dx%d!\n", $INFILE, $w, $h);
  exit(1);
}

$_ = fgets($fi); /* Read depth (making assumptions here!) */


/* So far so good; open output file! */
$fo = fopen($OUTFILE, "wb");
if ($fo == NULL) {
  fprintf(STDERR, "Cannot open '%s' for write!\n", $OUTFILE);
  exit(1);
}

for ($y = 0; $y < $h; $y++) {
  $line = fgets($fi); /* Assuming one '\n'-terminated line per row! */
  $data = explode(" ", $line);
  for ($x = 0; $x < $w; $x += 2) {
    $hi = $data[$x];
    $lo = $data[$x + 1];
    $b = (($hi / 17) * 16) + ($lo / 17);
    fputs($fo, chr($b), 1);
  }
}

fclose($fo);
fclose($fi);

