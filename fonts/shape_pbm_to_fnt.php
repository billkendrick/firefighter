#!/usr/bin/php
<?php

$fi = fopen("firefite-hirez-font.pbm", "rb");
$fo = fopen("fireshap.fnt", "wb");

$str = fgets($fi);
$str = fgets($fi);
$str = fgets($fi);

$font = array();

for ($y = 0; $y < 64; $y++) {
  $font[$y] = array();
  for ($x = 0; $x < 32; $x++) {
    $c1 = ord(fgetc($fi));
    $c2 = ord(fgetc($fi));

    $c = $c1 * 256 + $c2;

    $rb = 0b1000000000000000;
    $wb = 0b10000000;
    $o = 0;
    for ($b = 0; $b < 8; $b++) {
      if (($c & $rb) != 0) {
        $o |= $wb;
      }
      $rb = $rb >> 2;
      $wb = $wb >> 1;
    }

    // printf("%d,%d = %02x%02x = %04x (%016s) => %02x (%08s)\n", $x, $y, $c1, $c2, $c, decbin($c), $o, decbin($o));
    $font[$y][$x] = $o;
  }
}

/*
for ($y = 0; $y < 64; $y++) {
  $x = 9;
  printf("%3d %02x %08s\n", $font[$y][$x], $font[$y][$x], decbin($font[$y][$x]));
  if ($y % 8 == 7) { echo "\n"; }
}
*/

for ($set = 0; $set < 2; $set++) {
  $ffi = fopen("firetext.fnt", "rb");
  fwrite($fo, fread($ffi, 512));
  fclose($ffi);

  for ($row = 0; $row < 2; $row++) {
    for ($x = 0; $x < 32; $x++) {
      for ($y = 0; $y < 8; $y++) {
        $o = $font[($set * 32) + ($row * 16) + $y][$x];
        fputs($fo, chr($o));
        // printf("%08s\n", decbin($o));
      }
      // echo "\n";
    }
  }
}

fclose($fi);
fclose($fo);

