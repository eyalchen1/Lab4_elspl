all : link

link: compile
	gcc -g -m32 -Wall -o hexeditplus hexeditplus.o

compile:clean hexeditplus.c
	gcc -g -m32 -Wall -c -o hexeditplus.o hexeditplus.c 

clean:
	rm -f *.o hexeditplus