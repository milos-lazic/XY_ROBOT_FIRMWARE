# OBJECT FILES
objects = main.o \
          cmdproctask.o \
          motortask.o

app: $(objects)
	gcc -o app $(objects) -pthread libs/bcm2836/libbcm2836.a

main.o: main.c
	gcc -c -std=c99 main.c

cmdproctask.o: cmdproctask.c
	gcc -c -std=c99 cmdproctask.c

motortask.o: motortask.c
	gcc -c -std=c99 motortask.c

.PHONY: clean
clean:
	rm -f $(objects)

