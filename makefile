default: vector

vector: ArrayList.o Test.o
	gcc -o test ArrayList.o Test.o
	
ArrayList.o: ArrayList.c ArrayList.h
	gcc -c ArrayList.c

Test.o: Test.c ArrayList.h
	gcc -c Test.c
	
clean:
	rm -f *.o

# FILES = ArrayList.c Test.c 
# build: $(FILES)
# 	gcc -o test $(FILES)