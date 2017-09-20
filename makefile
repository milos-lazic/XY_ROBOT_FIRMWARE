app: main.o cmdproctask.o motortask.o
	gcc -o app main.o cmdproctask.o motortask.o -pthread libs/bcm2836/libbcm2836.a

main.o: main.c
	gcc -c -std=c99 main.c

cmdproctask.o: cmdproctask.c
	gcc -c -std=c99 cmdproctask.c

motortask.o: motortask.c
	gcc -c -std=c99 motortask.c

clean:
	rm -f *.o

