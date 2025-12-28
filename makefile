all : link

link: compile
	gcc -g -m32 -Wall -o hexeditplus hexeditplus.o
	gcc -g -m32 -Wall -o task4 task4.o

compile:clean hexeditplus.c task4.c
	gcc -g -m32 -Wall -c -o hexeditplus.o hexeditplus.c 
	gcc -g -m32 -fno-pie -fno-stack-protector -Wall -c -o task4.o task4.c


clean:
	rm -f *.o hexeditplus
	rm -f *.o task4