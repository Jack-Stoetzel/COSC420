a.out: main.o
	mpicc -o a.out main.o -lm

main.o: main.c matrixMath.h
	mpicc -c main.c -lm

clean:
	rm *.o a.out
