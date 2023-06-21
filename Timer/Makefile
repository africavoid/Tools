CC=cc
CFLAGS=-Wall -lncurses
SOURCES=main.c
EXE=timer
INSTALLDIR=/usr/local/bin/

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXE)
install:
	cp $(EXE) $(INSTALLDIR)
