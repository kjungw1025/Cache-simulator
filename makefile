CC = gcc -g
LIBS = -lstdc++

csim: main.o makecache.o
	gcc -o csim main.o makecache.o

makecache.o: makecache.c makecache.h
	gcc -c makecache.c -o makecache.o

main.o: main.c
	gcc -c main.c -o main.o

clean:
	del -f csim.exe *.o

