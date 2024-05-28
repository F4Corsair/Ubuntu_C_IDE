# 변수 정의
CC = gcc
CFLAGS = -lcurses -lpthread
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TARGET = main.out

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)

# 개별 오브젝트 파일 생성 규칙
%.o: %.c %.h
	$(CC) -c $< $(CFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

.PHONY: all clean
