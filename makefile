OBJS	= sockets.o
SOURCE	= simple-soket-core/sockets.c
HEADER	=
OUT	= sockets
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 =

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

sockets.o: simple-soket-core/sockets.c
	$(CC) $(FLAGS) simple-soket-core/sockets.c


clean:
	rm -f $(OBJS) $(OUT)
