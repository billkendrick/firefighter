# Makefile for firefight
#
# Firefighting game for the Atari 8-bit
# Bill Kendrick <bill@newbreedsoftware.com>
# http://www.newbreedsoftware.com/
#
# 2023-08-13 - 2023-08-19

CC65BIN=/usr/bin
CC65=${CC65BIN}/cc65
CA65=${CC65BIN}/ca65
LD65=${CC65BIN}/ld65
CC65_HOME=/usr/share/cc65
CC65_INC=${CC65_HOME}/include
CC65_ASMINC=${CC65_HOME}/asminc
CC65_LIB=${CC65_HOME}/lib
CC65_CFG=${CC65_HOME}/cfg
CC65_FLAGS=-Osir --add-source
LEVEL_FILES=$(wildcard levels/level*.txt)
OBJECTS=obj/firefite.o obj/segments.o obj/title.o obj/game.o obj/draw_text.o obj/dli.o

all:	firefite.xex

run:	firefite.xex
	atari800 -nobasic -run firefite.xex

clean:
	-rm firefite.xex
	-rm obj/*.o
	-rm firefite.s
	-rm dli.s
	-rm draw_text.s
	-rm title.s
	-rm game.s
	-rm firefite.map
	-rm levels.dat

firefite.xex:	${OBJECTS} src/atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o firefite.xex \
		-C src/atari.cfg \
		-m firefite.map \
		${OBJECTS} atari.lib

obj/firefite.o:  firefite.s
	${CA65} -I "${CC65_ASMINC}" -t atari firefite.s -o obj/firefite.o

firefite.s:  src/firefite.c src/title.h src/game.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/firefite.c -o firefite.s

obj/title.o:  title.s
	${CA65} -I "${CC65_ASMINC}" -t atari title.s -o obj/title.o

title.s:  src/title.c src/title.h src/shapes.h src/dli.h src/draw_text.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/title.c -o title.s

obj/game.o:  game.s
	${CA65} -I "${CC65_ASMINC}" -t atari game.s -o obj/game.o

game.s:  src/game.c src/game.h src/shapes.h src/dli.h src/draw_text.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/game.c -o game.s

obj/draw_text.o:  draw_text.s
	${CA65} -I "${CC65_ASMINC}" -t atari draw_text.s -o obj/draw_text.o

draw_text.s:  src/draw_text.c src/draw_text.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/draw_text.c -o draw_text.s

obj/dli.o:  dli.s
	${CA65} -I "${CC65_ASMINC}" -t atari dli.s -o obj/dli.o

dli.s:  src/dli.c src/dli.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari src/dli.c -o dli.s

obj/segments.o:     src/segments.s fonts/fire1.fnt fonts/fire2.fnt levels.dat
	${CA65} -I "${CC65_ASMINC}" -t atari src/segments.s -o obj/segments.o

levels.dat:	level_to_dat.php ${LEVEL_FILES}
	./level_to_dat.php ${LEVEL_FILES}

