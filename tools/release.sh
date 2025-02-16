#!/bin/sh

# "tools/release.sh" for firefight
#
# A script to build the game files and roll the source &
# game files into a release ZIP file.
#
# Firefighting game for the Atari 8-bit
# Bill Kendrick <bill@newbreedsoftware.com>
# http://www.newbreedsoftware.com/firefight/

# Last updated 2025-01-20

# Confirm that we've tagged with the same version
# number we're bulding it as.  (Makefile "VERSION"
# matches most recent "git tag".)  Use '-f' to force.

MAKE_VERSION=`grep "^VERSION=" Makefile | cut -d '=' -f 2`
README_VERSION=`grep "^### Version: " README.md | cut -d ' ' -f 3`
TAG=`git tag | sort -V | tail -1 | sed -e "s/[a-z]/\U&/g"`
echo "Version in Makefile:  $MAKE_VERSION"
echo "Version in README.md: $README_VERSION"
echo "Latest git repo tag:  $TAG"
echo

if [ "$MAKE_VERSION" != "$README_VERSION" ]; then
  echo "Mismatch!  Makefile version ($MAKE_VERSION) doesn't match"
  echo "README.md version ($README_VERSION)!  Correct that first!"
  exit
fi

if [ "$MAKE_VERSION" != "$TAG" ]; then
  if [ "$1" != "-f" ]; then
    VERSION_LOWER=`echo "$MAKE_VERSION" | sed -e "s/[A-Z]/\L&/g"`
    echo "Mismatch!  Use '-f' to ignore. Will use Makefile version # ($MAKE_VERSION)"
    echo "Or, if you're ready, run:"
    echo "  git tag $VERSION_LOWER"
    echo "  git push origin $VERSION_LOWER"
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

cd img-src ; make clean-intermediate ; cd ..
make clean
make
make clean-intermediate

# ZIP this directory!
cd ..
zip --exclude "*/.git/*" -r firefighter-${MAKE_VERSION}-`date +"%Y-%m-%d"`.zip firefighter/

echo
echo "Create a new release over at GitHub:"
echo 
echo "  https://github.com/billkendrick/firefighter/releases/new"
echo
echo "Post with this version's changes found in CHANGES.txt"
echo

