# Makefile for Firefighter
#
# Firefighting game for the Atari 8-bit
# Bill Kendrick <bill@newbreedsoftware.com>
# http://www.newbreedsoftware.com/firefight/
#
# 2023-08-13 - 2025-11-03

## Version number:
## (Note: Any alphabetic chars should be uppercase!)
VERSION=0.1-BETA11

## Run "tools/release.sh" to generate a release ZIP file
## (it will be named based on VERSION above,
## and the date (not from below)).

## Version date:
DATE=${shell /usr/bin/date +"%Y-%m-%d"}


## Binaries:
CC65BIN=/usr/bin
CC65=${CC65BIN}/cc65
CA65=${CC65BIN}/ca65
LD65=${CC65BIN}/ld65
DIR2ATR=/usr/local/bin/dir2atr -S -b MyDos4534
MARKDOWN2HTML=/usr/bin/markdown
HTML2TXT=/usr/bin/w3m -dump -no-graph -cols 38 -o indent_incr=1

## Include and Library Paths
CC65_HOME=/usr/share/cc65
CC65_INC=${CC65_HOME}/include
CC65_ASMINC=${CC65_HOME}/asminc
CC65_LIB=${CC65_HOME}/lib

## Flags
CC65_FLAGS=-O -Os -Oi -Or --static-locals --add-source
# MAP_ARGS=-m firefite.map

## Level 'source' files:
LEVEL_FILES=$(shell seq -f "levels/level%03g.txt" 1 12)

## Objects
OBJECTS_SHARED=obj/segments.o obj/game.o obj/shapes.o obj/draw_text.o
OBJECTS=obj/firefite.o obj/title.o obj/score.o obj/config.o obj/dli.o ${OBJECTS_SHARED}
OBJECTS_DISK=obj/firefite_disk.o obj/title_disk.o obj/score_disk.o obj/config_disk.o obj/dli_disk.o obj/help.o ${OBJECTS_SHARED}
OBJECTS_FUJINET=obj/firefite_fujinet.o obj/title_fujinet.o obj/score.o obj/dli.o obj/config_fujinet.o obj/app_key.o obj/sio.o ${OBJECTS_SHARED}

TMP=tmp-README.html


## Main Targets:
all:	firefite.atr firefite.xex fireftfn.xex

run:	firefite.xex
	atari800 -nobasic -run firefite.xex

run-fujinet:	fireftfn.xex
	atari800 -nobasic -run fireftfn.xex

run-disk:	firefite.atr
	atari800 -nobasic firefite.atr

clean:	clean-intermediate
	-rm firefite.xex
	-rm fireftfn.xex
	-rm firefite.atr

clean-intermediate:
	-rm obj/*.o
	-rm asm/*.s
	# -rm firefite.map
	-rm data/levels.dat
	-rm data/levels_cmp.dat
	-rm data/title.gr9
	-rm data/silh.dat
	-rm fonts/fireshap.fnt
	-rm firefths.xex
	-rm splash.xex
	-rm firefite-nohighscore.atr
	-rm disk/SPLASH.AR0
	-rm disk/FIREFITE.AR1
	-rm disk/README.txt
	-rm disk/TITLE.GR9


## Files to generate:

firefite.atr:	firefite-nohighscore.atr tools/high_score_atr.php
	tools/high_score_atr.php firefite-nohighscore.atr firefite.atr

firefite-nohighscore.atr:	firefths.xex splash.xex README.md data/title.gr9 data/silh.dat disk/README.txt
	cp splash.xex disk/SPLASH.AR0
	cp firefths.xex disk/FIREFITE.AR1
	cp data/title.gr9 disk/TITLE.GR9
	cp data/silh.dat disk/SILH.DAT
	${DIR2ATR} firefite-nohighscore.atr disk

firefite.xex:	${OBJECTS} src/atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o firefite.xex \
		-C src/atari.cfg \
		${MAP_ARGS} \
		${OBJECTS} atari.lib

firefths.xex:	${OBJECTS_DISK} src/atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o firefths.xex \
		-C src/atari.cfg \
		${MAP_ARGS} \
		${OBJECTS_DISK} atari.lib

fireftfn.xex:	${OBJECTS_FUJINET} src/atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o fireftfn.xex \
		-C src/atari.cfg \
		${MAP_ARGS} \
		${OBJECTS_FUJINET} atari.lib

splash.xex:	obj/splash.o obj/segments-splash.o src/atari-splash.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o splash.xex \
		-C src/atari-splash.cfg \
		obj/splash.o obj/segments-splash.o atari.lib

## Source code to compile and/or assemble:

# Main:
# -----
obj/firefite.o:  asm/firefite.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/firefite.s -o obj/firefite.o

asm/firefite.s:  src/firefite.c src/title.h src/game.h src/score.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/firefite.c -o asm/firefite.s

obj/firefite_disk.o:  asm/firefite_disk.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/firefite_disk.s -o obj/firefite_disk.o

asm/firefite_disk.s:  src/firefite.c src/title.h src/game.h src/score.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari -D DISK src/firefite.c -o asm/firefite_disk.s

obj/firefite_fujinet.o:  asm/firefite_fujinet.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/firefite_fujinet.s -o obj/firefite_fujinet.o

asm/firefite_fujinet.s:  src/firefite.c src/title.h src/game.h src/score.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari -D FUJINET src/firefite.c -o asm/firefite_fujinet.s

# Title and Help Screens:
# -----------------------
obj/title.o:  asm/title.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/title.s -o obj/title.o

asm/title.s:  src/title.c src/title.h src/shapes.h src/dli.h src/draw_text.h Makefile
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari -D VERSION=\"${VERSION}\" -D DATE=\"${DATE}\" src/title.c -o asm/title.s

obj/title_disk.o:  asm/title_disk.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/title_disk.s -o obj/title_disk.o

asm/title_disk.s:  src/title.c src/title.h src/shapes.h src/dli.h src/draw_text.h Makefile
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari -D DISK -D VERSION=\"${VERSION}-DISK\" -D DATE=\"${DATE}\" src/title.c -o asm/title_disk.s

obj/title_fujinet.o:  asm/title_fujinet.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/title_fujinet.s -o obj/title_fujinet.o

asm/title_fujinet.s:  src/title.c src/title.h src/shapes.h src/dli.h src/draw_text.h Makefile
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari -D FUJINET -D VERSION=\"${VERSION}-FUJINET\" -D DATE=\"${DATE}\" src/title.c -o asm/title_fujinet.s

obj/help.o:  asm/help.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/help.s -o obj/help.o

asm/help.s:  src/help.c src/help.h src/draw_text.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/help.c -o asm/help.s

# Config and I/O:
# ---------------
obj/config.o:  asm/config.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/config.s -o obj/config.o

asm/config.s:  src/config.c src/config.h src/draw_text.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/config.c -o asm/config.s

obj/config_disk.o:  asm/config_disk.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/config_disk.s -o obj/config_disk.o

asm/config_disk.s:  src/config.c src/config.h src/draw_text.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari -D DISK src/config.c -o asm/config_disk.s

obj/config_fujinet.o:  asm/config_fujinet.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/config_fujinet.s -o obj/config_fujinet.o

asm/config_fujinet.s:  src/config.c src/config.h src/draw_text.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari -D FUJINET src/config.c -o asm/config_fujinet.s

# High Score Recording and Top-10 Screen:
# ---------------------------------------
obj/score.o:  asm/score.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/score.s -o obj/score.o

asm/score.s:  src/score.c src/score.h src/game.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/score.c -o asm/score.s

obj/score_disk.o:  asm/score_disk.s src/game.h
	${CA65} -I "${CC65_ASMINC}" -t atari asm/score_disk.s -o obj/score_disk.o

asm/score_disk.s:  src/score.c src/score.h src/draw_text.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari -D DISK src/score.c -o asm/score_disk.s

# FujiNet High Score and Settings I/O:
# ------------------------------------
obj/app_key.o:	asm/app_key.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/app_key.s -o obj/app_key.o

asm/app_key.s:  src/app_key.c src/app_key.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/app_key.c -o asm/app_key.s

obj/sio.o:  src/sio.s
	${CA65} -I "${CC65_ASMINC}" -t atari src/sio.s -o obj/sio.o

# Help Text:
# ----------
# * Convert `code` to ~code~, which will become Inverse ATASCII text
# * Convert ASCII EOL to ATASCII EOL
# * Convert " * " and " o " bullet glyphs to ATASCII control characters
#   (circle and diamond graphics, respectively)
disk/README.txt:	README.md tools/txt2atascii.php
	cat README.md | tr "\`" "\~" | ${MARKDOWN2HTML} > ${TMP}; \
		${HTML2TXT} ${TMP} \
		| tools/txt2atascii.php \
		| sed -e "s/ \* / \x14 /g" \
		| sed -e "s/ o / \x60 /g" \
		> disk/README.txt
	-rm ${TMP}


# Game loop:
# ----------
obj/game.o:  asm/game.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/game.s -o obj/game.o

asm/game.s:  src/game.c src/game.h src/shapes.h src/dli.h src/draw_text.h src/score.h src/config.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/game.c -o asm/game.s

# Shape table
# -----------
obj/shapes.o:  asm/shapes.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/shapes.s -o obj/shapes.o

asm/shapes.s:  src/shapes.c src/shapes.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/shapes.c -o asm/shapes.s

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

obj/dli_disk.o:  asm/dli_disk.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/dli_disk.s -o obj/dli_disk.o

asm/dli_disk.s:  src/dli.c src/dli.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari -D DISK src/dli.c -o asm/dli_disk.s

# Data segments:
# --------------
obj/segments.o:     src/segments.s fonts/firetext.fnt fonts/fireshap.fnt data/levels_cmp.dat
	${CA65} -I "${CC65_ASMINC}" -t atari src/segments.s -o obj/segments.o

# Splash:
# -------
obj/splash.o:  asm/splash.s
	${CA65} -I "${CC65_ASMINC}" -t atari asm/splash.s -o obj/splash.o

asm/splash.s:  src/splash.c
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/splash.c -o asm/splash.s

obj/segments-splash.o:     src/segments-splash.s
	${CA65} -I "${CC65_ASMINC}" -t atari src/segments-splash.s -o obj/segments-splash.o

## Data files to build:

# Levels:
# -------
data/levels_cmp.dat:	tools/level_compress.php tools/level_consts.inc.php data/levels.dat
	./tools/level_compress.php

data/levels.dat:	tools/level_to_dat.php tools/level_consts.inc.php ${LEVEL_FILES}
	./tools/level_to_dat.php ${LEVEL_FILES}

# Splash title screen:
# --------------------
data/title.gr9:	img-src/title.pgm tools/pgm2gr9.php
	tools/pgm2gr9.php

data/silh.dat:	img-src/silh.pbm tools/pbm2pmg.php
	tools/pbm2pmg.php

# Font:
# -----
fonts/fireshap.fnt: fonts/firefite-hirez-font.pbm fonts/firetext.fnt tools/shape_pbm_to_fnt.php
	tools/shape_pbm_to_fnt.php fonts/firefite-hirez-font.pbm fonts/fireshap.fnt fonts/firetext.fnt

