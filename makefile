OBJS	= obj/sockets.o
SOURCE	= sources/simple-soket-core/sockets.c
INC_DIR = ./header
HEADER	= 
OUT	= bin/sockets
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -I. -I$(INC_DIR)

OBJS_TEST	= obj/test-core.o obj/test-testCore.o obj/test-all.o
SOURCE_TEST	= sources/test/test-all.c sources/test/test-core.c sources/test/test-testCore.c
HEADER_TEST	= header/all-test.h header/test-core.h
OUT_TEST	= bin/test

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

obj/sockets.o: sources/simple-soket-core/sockets.c
	mkdir -p obj bin
	$(CC) $(FLAGS) sources/simple-soket-core/sockets.c -o obj/sockets.o


clean:
	rm -f $(OBJS) $(OUT)


test: $(OBJS_TEST)
	$(CC) -g $(OBJS_TEST) -o $(OUT_TEST)

obj/test-core.o: sources/test/test-core.c header/test-core.h
	mkdir -p obj bin
	$(CC) $(FLAGS) sources/test/test-core.c -o obj/test-core.o $(LFLAGS)

obj/test-testCore.o: sources/test/test-testCore.c header/test-core.h 
	mkdir -p obj bin
	$(CC) $(FLAGS) sources/test/test-testCore.c -o obj/test-testCore.o $(LFLAGS)

obj/test-all.o: sources/test/test-all.c header/all-test.h
	mkdir -p obj bin
	$(CC) $(FLAGS) sources/test/test-all.c -o obj/test-all.o $(LFLAGS)

clean_test:
	rm -f $(OBJS_TEST) $(OUT_TEST)