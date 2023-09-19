# Makefile for firefight's title screen data
#
# Firefighting game for the Atari 8-bit
# Bill Kendrick <bill@newbreedsoftware.com>
# http://www.newbreedsoftware.com/firefight/
#
# 2023-08-13 - 2023-09-15

all:	title.pgm

clean:
	-rm title.png
	-rm title-scaled.png
	-rm title-fullsize1.png
	-rm title-fullsize2.png
	-rm label.png


title.pgm:	title-scaled.png greys16.pgm
	convert title-scaled.png -remap greys16.pgm \
		-compress None title.pgm

title-scaled.png: title-fullsize2.png
	convert title-fullsize2.png -resize "80!x192" title-scaled.png

title-fullsize2.png: title-fullsize1.png 232353.png
	convert -composite -gravity southeast title-fullsize1.png 232353.png \
		title-fullsize2.png

title-fullsize1.png: firefighter-16549150074BW.jpg label.png
	convert -composite -gravity center firefighter-16549150074BW.jpg label.png \
		title-fullsize1.png

label.png:
	convert -size 640x400 xc:transparent \
		-pointsize 110 -font "Echotopia-Regular" -fill white \
		-annotate +15+95 'FIREFIGHTER' \
		-pointsize 110 -font "Echotopia-Regular" -fill black \
		-annotate +10+90 'FIREFIGHTER' \
		\
		-pointsize 90 -font "K.O.-Activista*-Bold" -fill black \
		-annotate +15+205 'BY BILL KENDRICK' \
		-pointsize 90 -font "K.O.-Activista*-Bold" -fill black \
		-annotate +5+205 'BY BILL KENDRICK' \
		-pointsize 90 -font "K.O.-Activista*-Bold" -fill black \
		-annotate +15+195 'BY BILL KENDRICK' \
		-pointsize 90 -font "K.O.-Activista*-Bold" -fill black \
		-annotate +5+195 'BY BILL KENDRICK' \
		-pointsize 90 -font "K.O.-Activista*-Bold" -fill white \
		-annotate +10+200 'BY BILL KENDRICK' \
		\
		label.png 

