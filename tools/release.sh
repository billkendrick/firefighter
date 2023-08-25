#!/bin/sh

# "tools/release.sh" for firefight
#
# A script to build the game files and roll the source &
# game files into a release ZIP file.
#
# Firefighting game for the Atari 8-bit
# Bill Kendrick <bill@newbreedsoftware.com>
# http://www.newbreedsoftware.com/firefight/

# Last updated 2023-08-25

# Confirm that we've tagged with the same version
# number we're bulding it as.  (Makefile "VERSION"
# matches most recent "git tag".)  Use '-f' to force.

VERSION=`grep "^VERSION=" Makefile | cut -d '=' -f 2`
TAG=`git tag | tail -1 | sed -e "s/[a-z]/\U&/g"`
echo "Version in Makefile: $VERSION"
echo "Latest git repo tag: $TAG"
echo

if [ "$VERSION" != "$TAG" ]; then
  if [ "$1" != "-f" ]; then
    echo "Mismatch!  Use '-f' to ignore. Will use Makefile version # ($VERSION)"
    echo "Or, if you're ready, run:"
    echo "  git tag $VERSION"
    echo "  git push origin $VERSION"
    echo
    echo "Here is the current 'git status':"
    echo "------------------------------------------------------------------------"
    git status
    echo "------------------------------------------------------------------------"
    echo
    exit
  fi
fi

# Clean build, and then remove every intermediary file built, leaving
# only the source code and the files end-users care about
# (the stand alone XEX executable and the ATR disk image).

make clean
make
make clean-intermediate

# ZIP this directory!
cd ..
zip --exclude "*/.git/*" -r firefighter-${VERSION}-`date +"%Y-%m-%d"`.zip firefighter/

echo
echo "Create a new release over at GitHub:"
echo 
echo "  https://github.com/billkendrick/firefighter/releases/new"
echo
echo "Post with this version's changes found in CHANGES.txt"
echo

