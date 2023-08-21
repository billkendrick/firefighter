#!/usr/bin/php
<?php
/*
  Creates levels for Firefighter

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/

  2023-08-18 - 2023-08-20
*/

$ascii_to_shape = array(
  " " => (0), // blank (floor)
  "|" => (1 + 64), // PIPE_UP_DOWN (1 + 64)
  ":" => (2 + 64), // PIPE_BROKEN_UP_DOWN (2 + 64)
  "-" => (3 + 64), // PIPE_LEFT_RIGHT (3 + 64)
  "=" => (4 + 64), // PIPE_BROKEN_LEFT_RIGHT (4 + 64)

  "{" => (5 + 64), // PIPE_DOWN_RIGHT (5 + 64)
  /* /- */
  /* |  */

  "}" => (6 + 64), // PIPE_DOWN_LEFT (6 + 64)
  /* -\  */
  /*  |  */

  "]" => (7 + 64), // PIPE_UP_LEFT (7 + 64)
  /*  |  */
  /* -/  */

  "[" => (8 + 64), // PIPE_UP_RIGHT (8 + 64)
  /*  |  */
  /*  \- */

  "+" => (9 + 64), // VALVE_OPEN (9 + 64)
  "X" => (10 + 64), // VALVE_CLOSED (10 + 64)

  "x" => (11 + 64), // CIVILIAN (11 + 64)

  "O" => (16 + 64), // OIL (16 + 64)

  "C" => (17 + 192), // CRATE (17 + 192)
  "c" => (18 + 192), // CRATE_BROKEN (18 + 192)

  "A" => (21 + 64), // AX (21 + 64)

  "," => (22), // FIRE_SM 22
  ";" => (23), // FIRE_MD 23
  "*" => (24), // FIRE_LG 24

  "D" => (26 + 192), // DOOR (26 + 192)

  "E" => (27 + 192), // EXIT1 (27 + 192)
  "e" => (28 + 192), // EXIT2 (28 + 192)

  "#" => (25 + 192), // WALL (25 + 192)
);

$OUTFILE = "data/levels.dat";
$fo = fopen($OUTFILE, "wb");
if ($fo == NULL) {
  fprintf(STDERR, "Cannot open %s for write!\n", $OUTFILE);
  exit(1);
}

/* How many levels will there be? */
fputs($fo, chr($argc - 1), 1);

$LEVEL_W = 20;
$LEVEL_H = 11;

/* Process each level file */
for ($i = 1; $i < $argc; $i++) {
  $fireman_x = -1;
  $fireman_y = -1;

  $fi = fopen($argv[$i], "r");
  if ($fi == NULL) {
    fprintf(STDERR, "Cannot open %s!\n", $argv[$i]);
    level_abort();
  }

  echo $argv[$i] . ":\n";
  for ($y = 0; $y < $LEVEL_H; $y++) {
    $line = rtrim(fgets($fi), "\n\r");
    if (feof($fi)) {
      fprintf(STDERR, "%s is fewer than %d lines (it's %d)!\n",
        $argv[$i], $LEVEL_H, $y);
      level_abort();
    }

    echo $line . "\n";

    if (strlen($line) != $LEVEL_W) {
      fprintf(STDERR, "Line %d of %s is not %d characters (it's %d)!\n",
        $y + 1, $argv[$i], $LEVEL_W, strlen($line));
      level_abort();
    }

    for ($x = 0; $x < $LEVEL_W; $x++) {
      $c = substr($line, $x, 1);
      if ($c == 'F') {
        $c = ' ';
        if ($fireman_x != -1) {
          fprintf(STDERR, "Line %d of %s contains another 'F' fireman at position %d; only one allowed per level!\n",
            $y + 1, $argv[$i], $x + 1);
          level_abort();
        }

        $fireman_x = $x;
        $fireman_y = $y;
      }

      if (!array_key_exists($c, $ascii_to_shape)) {
        fprintf(STDERR, "Line %d of %s contains unknown character '%c' at position %d!\n",
          $y + 1, $argv[$i], $c, $x + 1);
        level_abort();
      }

      fputs($fo, chr($ascii_to_shape[$c]), 1);
    }
  }

  if ($fireman_x == -1) {
    fprintf(STDERR, "%s did not have an 'F' fireman location!\n", $argv[$i]);
    level_abort();
  }

  printf("Fireman @ (%d,%d)\n", $fireman_x, $fireman_y);
  fputs($fo, chr($fireman_x), 1);
  fputs($fo, chr($fireman_y), 1);

  echo "\n";
}

fclose($fo);

function level_abort() {
  global $OUTFILE;

  fprintf(STDERR, "Deletng %s\n", $OUTFILE);
  unlink($OUTFILE);
  exit(1);
}

