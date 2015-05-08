default: vector

vector: ArrayList.o Test.o
	gcc -std=c99 -o test ArrayList.o Test.o
	
ArrayList.o: ArrayList.c ArrayList.h
	gcc -std=c99 -c ArrayList.c

Test.o: Test.c ArrayList.h
	gcc -std=c99 -c Test.c
	
clean:
	rm -f *.o
