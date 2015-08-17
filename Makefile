IDIR = include
CC = gcc
CFLAGS = -I $(IDIR)

SDIR = src

ODIR = $(SDIR)/obj

_DEPS = input.h array.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = threes.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

LIBS = -lm

$(ODIR)/%.o: src/%.c $(DEPS)
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

threes: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

run: threes
	./threes

.PHONY: clean
clean:
	rm -f threes $(ODIR)/*.o *~ $(IDIR)/*~
