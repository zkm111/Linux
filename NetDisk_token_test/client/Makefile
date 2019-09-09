SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=client
CC:=gcc
CFLAGS:=-g -Wall -D DEBUG
$(ELF):$(OBJS)
	gcc $^ -o $@ -lcrypt -lpthread
.PHONY:clean
clean:
	rm -rf $(ELF) $(OBJS)

