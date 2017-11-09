
CC = gcc
CFLAGS = -std=c99 -Wall
EXECUTABLE = app

LIBS = -pthread \
       -lwiringPi \
       libs/bcm2836/libbcm2836.a

SRC = cmdproctask.c \
      motortask.c \
      mtlookup.c \
      mtservo.c \
      main.c

OBJ = $(SRC:.c=.o)

BIN = $(SRC:.c=)




$(EXECUTABLE): $(OBJ)
	@ echo [LINK]
	@ $(CC) -o app $(OBJ) $(LIBS)

main.o: main.c
	@ echo [CC $<]
	@ $(CC) -c $(CFLAGS) $<

cmdproctask.o: cmdproctask.c
	@ echo [CC $<]
	@ $(CC) -c $(CFLAGS) $<

motortask.o: motortask.c
	@ echo [CC $<]
	@ $(CC) -c $(CFLAGS) $<

mtlookup.o : mtlookup.c
	@ echo [CC $<]
	@ $(CC) -c $(CFLAGS) $<

mtservo.o : mtservo.c
	@ echo [CC $<]
	@ $(CC) -c $(CFLAGS) $<


.PHONY: clean
clean:
	@ echo [Clean]
	@ rm -f $(OBJ)

