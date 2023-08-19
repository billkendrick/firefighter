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
	-rm segments.o
	-rm firefite.map

firefite.xex:	firefite.o segments.o atari.cfg
	${LD65} --lib-path "${CC65_LIB}" \
		-o firefite.xex \
		-t atari \
		-m firefite.map \
		firefite.o segments.o atari.lib

firefite.o:  firefite.s
	${CA65} -I "${CC65_ASMINC}" -t atari firefite.s -o firefite.o

firefite.s:  firefite.c
	${CC65} ${CC65_FLAGS} -I "${CC65_INC}" \
		-t atari \
		firefite.c \
		-o firefite.s

segments.o:     segments.s fire1.fnt fire2.fnt
	${CA65} -I "${CC65_ASMINC}" -t atari segments.s -o segments.o

