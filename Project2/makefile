all: search1 search2

search1: main.o
	mpicc -o search1 main.o

main.o: main.c atree.h wtree.h
	mpicc -c -g main.c

search2: main2.o
	gcc -o search2 main2.o

main2.o: main2.c atree.h wtree.h
	gcc -c main2.c

clean:
	rm search1 search2 *.o
	clear
