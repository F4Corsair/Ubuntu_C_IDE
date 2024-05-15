CC=gcc
CFLAGS=-g
LIBS=-lcurses

layout.out: layout.c ls.c
    $(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
    rm -f layout.out
