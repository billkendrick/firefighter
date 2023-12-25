#!/usr/bin/php
<?php
/*
  Converts plaintext from Unix to ATASCII

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-12-24 - 2023-12-24
*/

$inv = 0;

while (!feof(STDIN)) {
  $c = fgetc(STDIN);
  if (ord($c) == 10) {
    echo chr(155);
  } else if ($c == '~') {
    $inv = !$inv;
  } else {
    if ($inv) {
      $c = chr(ord($c) + 128);
    }
    echo $c;
  }
}

