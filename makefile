OBJS	= obj/sockets.o
SOURCE	= sources/simple-soket-core/sockets.c
HEADER	= 
OUT	= bin/sockets
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 =

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

obj/sockets.o: sources/simple-soket-core/sockets.c
	mkdir -p obj bin
	$(CC) $(FLAGS) sources/simple-soket-core/sockets.c -o obj/sockets.o


clean:
	rm -f $(OBJS) $(OUT)
