CC=g++
CFLAGS= -W -Wall -Wextra -O2 -g -Werror
LIBS=-lSDL2 -lSDL2_image

SRC=$(wildcard *.cpp)
OBJS=$(SRC:.cpp=.o)
AOUT=main

all: main clean_obj

main: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< $(LIBS)

clean_obj:
	@rm *.o

clean:
	@rm $(AOUT)

