# if you want to build without debug flag
# try $ make DEBUG=0
CC = gcc
DEBUG ?= 1
CFLAGS = -lcurses
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

ifeq ($(DEBUG), 1)
	CFLAGS += -g
endif

main.out : $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)

%.o : %.c %.h
	$(CC) -c $< $(CFLAGS)

%.o : %.c
	$(CC) -c $< $(CFLAGS)

.PHONY : clean
clean :
	rm -f $(OBJS) main.out
