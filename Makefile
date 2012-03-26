
CC=clang
CFLAGS=-Wall -g -c
LDFLAGS=-lncurses
SOURCES=src/mftimer.c \
	src/mffilesystem.c \
	src/mflog.c \
	src/mfplayer.c \
	src/mfenemies.c \
	src/mfworld.c \
	src/mfgame.c \
	src/minefield.c
OBJECTS=$(SOURCES:.c=.o)
EXE=minefield

all: $(SOURCES) $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS)
	rm $(EXE)
