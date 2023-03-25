# Makefile

IDIR =../include
CC=g++
CFLAGS=-I$(IDIR)

ODIR=obj
#LDIR =../lib

LIBS=-lncurses -ldl -lm -lpthread

_DEPS = nseq.h clip.h track.h audioEngine.h miniaudio.h sequencer.h fileio.h engineQueue.h nsequeue.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = nseq.o clip.o track.o gui.o audioEngine.o miniaudio.o sequencer.o fileio.o engineQueue.o nsequeue.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

nseq: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
