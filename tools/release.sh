#!/bin/sh

make clean
make
make clean-intermediate

cd ..
zip --exclude "*/.git/*" -r firefighter-`date +"%Y-%m-%d"`.zip firefighter/

