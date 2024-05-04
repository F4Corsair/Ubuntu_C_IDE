CC = gcc
CFLAGS = -g -lcurses
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

main.out : $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)

%.o : %.c %.h
	$(CC) -c $< $(CFLAGS)

%.o : %.c
	$(CC) -c $< $(CFLAGS)

.PHONY : clean
clean :
	rm -f $(OBJS) main.out
