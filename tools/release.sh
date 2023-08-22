#!/bin/sh

make clean
make
make clean-intermediate

VERSION=`grep "^VERSION=" Makefile | cut -d '=' -f 2`

cd ..
zip --exclude "*/.git/*" -r firefighter-${VERSION}-`date +"%Y-%m-%d"`.zip firefighter/

