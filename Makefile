CC := gcc

OBJS := ./src/main.c ./src/cpp_auto_generate.c
INCS := -Iinclude
CFLAGS := -Wall -lm

OBJ_NAME := main

all : $(OBJS)
	$(CC) $(OBJS) $(INCS) $(CFLAGS) -o $(OBJ_NAME)

clean :
	rm $(OBJ_NAME)

.PHONY : clean