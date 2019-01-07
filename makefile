all: bag main.o bow.o mymalloc.o

bag: bow.o main.o mymalloc.o
	gcc -Wall -ansi -pedantic bow.o main.o mymalloc.o -o bag

main.o: main.c bow.h mymalloc.h
	gcc -Wall -ansi -pedantic -c main.c -o main.o

bow.o: bow.c bow.h mymalloc.h
	gcc -Wall -ansi -pedantic -c bow.c -o bow.o

mymalloc.o: mymalloc.c bow.h mymalloc.h
	gcc -Wall -ansi -pedantic -c mymalloc.c -o mymalloc.o

clean:
	rm -i bag bow.o main.o mymalloc.o
