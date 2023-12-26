#!/usr/bin/php
<?php
/*
  Converts title screen silhouette from NetPBM portable bitmap (PBM)
  to data to utilize as five Player/Missile Graphics (sprite) objects

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-12-26 - 2023-12-26
*/

$INFILE = "img-src/silh.pbm";
$OUTFILE = "data/silh.dat";

/* Open input file */
$fi = fopen($INFILE, "r");
if ($fi == NULL) {
  fprintf(STDERR, "Cannot open '%s' for read!\n", $INFILE);
  exit(1);
}

/*
P1
40 192
*/

/* Get & check header */
$line = fgets($fi);
if (trim($line) != "P1") {
  fprintf(STDERR, "Input '%s' is not a 'P1' PBM file!\n", $INFILE);
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
if ($w != 40 || $h != 192) {
  fprintf(STDERR, "Input '%s' is not 40x192; it's %dx%d %d!\n", $INFILE, $w, $h);
  exit(1);
}


/* So far so good; open output file! */
$fo = fopen($OUTFILE, "wb");
if ($fo == NULL) {
  fprintf(STDERR, "Cannot open '%s' for write!\n", $OUTFILE);
  exit(1);
}

$SKIP = 48;

/* Skip the part above the firefighter's silhouette */

for ($y = 0; $y < $SKIP; $y++) {
  $line = fgets($fi); /* Assuming one '\n'-terminated line per row! */
}

$rows = array();

for ($y = 0; $y < $h - $SKIP; $y++) {
  $line = fgets($fi); /* Assuming one '\n'-terminated line per row! */
  $data = explode(" ", $line);

  $rows[$y] = array();

  for ($ply = 0; $ply < 5; $ply++) {
    $bits = 0;
    $mult = 128;
    for ($x = 0; $x < 8; $x++) {
      $bits += ($data[$ply * 8 + $x] * $mult);
      $mult = $mult / 2;
    }
    $rows[$y][$ply] = $bits;
  }
}

for ($ply = 0; $ply < 5; $ply++) {
  for ($y = 0; $y < $h - $SKIP; $y++) {
    $b = $rows[$y][$ply];
    fputs($fo, chr($b), 1);
  }
}

fclose($fo);
fclose($fi);

