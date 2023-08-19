# Makefile for firefight
#
# Firefighting game for the Atari 8-bit
# Bill Kendrick <bill@newbreedsoftware.com>
# http://www.newbreedsoftware.com/
#
# 2023-08-13 - 2023-08-13

CC65BIN=/usr/bin
CC65=${CC65BIN}/cc65
CA65=${CC65BIN}/ca65
LD65=${CC65BIN}/ld65
CC65_HOME=/usr/share/cc65/
CC65_INC=${CC65_HOME}/include/
CC65_ASMINC=${CC65_HOME}/asminc/
CC65_LIB=${CC65_HOME}/lib/
CC65_CFG=${CC65_HOME}/cfg/
CC65_FLAGS=-Osir --add-source

all:	firefite.xex

run:	firefite.xex
	atari800 -nobasic -run firefite.xex

clean:
	-rm firefite.xex
	-rm firefite.o
	-rm firefite.s
	-rm dli.o
	-rm dli.s
	-rm draw_text.o
	-rm draw_text.s
	-rm title.o
	-rm title.s
	-rm segments.o
	-rm firefite.map

firefite.xex:	firefite.o segments.o title.o draw_text.o dli.o atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o firefite.xex \
		-t atari \
		-m firefite.map \
		firefite.o segments.o title.o draw_text.o dli.o atari.lib

firefite.o:  firefite.s
	${CA65} -I "${CC65_ASMINC}" -t atari firefite.s -o firefite.o

firefite.s:  firefite.c shapes.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari firefite.c -o firefite.s

title.o:  title.s
	${CA65} -I "${CC65_ASMINC}" -t atari title.s -o title.o

title.s:  title.c shapes.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari title.c -o title.s

draw_text.o:  draw_text.s
	${CA65} -I "${CC65_ASMINC}" -t atari draw_text.s -o draw_text.o

draw_text.s:  draw_text.c shapes.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari draw_text.c -o draw_text.s

dli.o:  dli.s
	${CA65} -I "${CC65_ASMINC}" -t atari dli.s -o dli.o

dli.s:  dli.c shapes.h
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" -t atari dli.c -o dli.s

segments.o:     segments.s fire1.fnt fire2.fnt
	${CA65} -I "${CC65_ASMINC}" -t atari segments.s -o segments.o

