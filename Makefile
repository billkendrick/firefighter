# Makefile for firefight
#
# Firefighting game for the Atari 8-bit
# Bill Kendrick <bill@newbreedsoftware.com>
# http://www.newbreedsoftware.com/firefight/
#
# 2023-08-13 - 2023-08-22

## Version number:
## (Note: Any alphabetic chars should be uppercase!)
VERSION=0.1-BETA1
DATE=${shell /usr/bin/date +"%Y-%m-%d"}

## Binaries:
CC65BIN=/usr/bin
CC65=${CC65BIN}/cc65
CA65=${CC65BIN}/ca65
LD65=${CC65BIN}/ld65
DIR2ATR=/usr/local/bin/dir2atr -S -b MyDos4534
MARKDOWN2HTML=/usr/bin/markdown
HTML2TXT=/usr/bin/w3m -dump -no-graph -cols 40
TXT2ATASCII=/usr/bin/tr "\n" "\233"

## Include and Library Paths
CC65_HOME=/usr/share/cc65
CC65_INC=${CC65_HOME}/include
CC65_ASMINC=${CC65_HOME}/asminc
CC65_LIB=${CC65_HOME}/lib

## Flags
CC65_FLAGS=-Osir --add-source
# MAP_ARGS=-m firefite.map

## Sources
LEVEL_FILES=$(wildcard levels/level*.txt)
OBJECTS=obj/firefite.o obj/segments.o obj/title.o obj/game.o obj/draw_text.o obj/dli.o
OBJECTS_DISK=obj/firefite_disk.o obj/segments.o obj/title_disk.o obj/game.o obj/draw_text.o obj/dli.o obj/help.o

TMP=tmp-README.html


## Main Targets:
all:	firefite.atr firefite.xex

run:	firefite.xex
	atari800 -nobasic -run firefite.xex

run-disk:	firefite.atr
	atari800 -nobasic firefite.atr

clean:	clean-intermediate
	-rm firefite.xex
	-rm firefite.atr
	-rm disk/FIREFITE.AR0
	-rm disk/README.txt

clean-intermediate:
	-rm obj/*.o
	-rm asm/*.s
	-rm firefite.map
	-rm data/levels.dat
	-rm firefths.xex


## Files to generate:

firefite.atr:	firefths.xex README.md
	cp firefths.xex disk/FIREFITE.AR0
	${MARKDOWN2HTML} README.md > ${TMP}; ${HTML2TXT} ${TMP} | ${TXT2ATASCII} > disk/README.txt
	rm ${TMP}
	${DIR2ATR} firefite.atr disk

firefite.xex:	${OBJECTS} src/atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o firefite.xex \
		-C src/atari.cfg \
		${MAP_ARGS} \
		${OBJECTS} atari.lib

# FIXME: High score version
firefths.xex:	${OBJECTS_DISK} src/atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o firefths.xex \
		-C src/atari.cfg \
		${MAP_ARGS} \
		${OBJECTS_DISK} atari.lib


## Source code to compile and/or assemble:

# Main:
# -----
obj/firefite.o:  asm/firefite.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/firefite.s -o obj/firefite.o

asm/firefite.s:  src/firefite.c src/title.h src/game.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/firefite.c -o asm/firefite.s

obj/firefite_disk.o:  asm/firefite_disk.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/firefite_disk.s -o obj/firefite_disk.o

asm/firefite_disk.s:  src/firefite.c src/title.h src/game.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari -D DISK src/firefite.c -o asm/firefite_disk.s

# Title and Help Screens:
# -----------------------
obj/title.o:  asm/title.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/title.s -o obj/title.o

asm/title.s:  src/title.c src/title.h src/shapes.h src/dli.h src/draw_text.h Makefile
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari -D VERSION=\"${VERSION}\" -D DATE=\"${DATE}\" src/title.c -o asm/title.s

obj/title_disk.o:  asm/title_disk.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/title_disk.s -o obj/title_disk.o

asm/title_disk.s:  src/title.c src/title.h src/shapes.h src/dli.h src/draw_text.h Makefile
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari -D DISK -D VERSION=\"${VERSION}-d\" -D DATE=\"${DATE}\" src/title.c -o asm/title_disk.s

obj/help.o:  asm/help.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/help.s -o obj/help.o

asm/help.s:  src/help.c src/help.h src/draw_text.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/help.c -o asm/help.s

# Game loop:
# ----------
obj/game.o:  asm/game.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/game.s -o obj/game.o

asm/game.s:  src/game.c src/game.h src/shapes.h src/dli.h src/draw_text.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/game.c -o asm/game.s

# Text drawing:
# -------------
obj/draw_text.o:  asm/draw_text.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/draw_text.s -o obj/draw_text.o

asm/draw_text.s:  src/draw_text.c src/draw_text.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/draw_text.c -o asm/draw_text.s

# Dislay List Interrupt:
# ----------------------
obj/dli.o:  asm/dli.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/dli.s -o obj/dli.o

asm/dli.s:  src/dli.c src/dli.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/dli.c -o asm/dli.s

# Data segments:
# --------------
obj/segments.o:     src/segments.s fonts/fire1.fnt fonts/fire2.fnt data/levels.dat
	${CA65} -I "${CC65_ASMINC}" -t atari src/segments.s -o obj/segments.o


## Data files to build:

data/levels.dat:	tools/level_to_dat.php ${LEVEL_FILES}
	./tools/level_to_dat.php ${LEVEL_FILES}

