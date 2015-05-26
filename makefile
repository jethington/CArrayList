default: vector

vector: vector.o test.o
	gcc -std=c99 -o test vector.o test.o
	
vector.o: vector.c vector.h
	gcc -std=c99 -c vector.c

test.o: test.c vector.h
	gcc -std=c99 -c test.c
	
clean:
	rm -f *.o
