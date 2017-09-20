define COMPILE_SOURCE
gcc -c -std=c99 $(1);
endef

# list of object files
#OBJECTS := bcm2836.o \
#           bcm2836_UART.o \
#           bcm2836_GPIO.o \
#           bcm2836_i2c.o \
#           bcm2836_spi.o

# list of source files
SOURCES := bcm2836.c \
           bcm2836_UART.c \
           bcm2836_GPIO.c \
           bcm2836_i2c.c \
           bcm2836_spi.c


# generate object files
objs:
	$(foreach source, $(SOURCES), $(call COMPILE_SOURCE, $(source)))

# generate test application executable
app: main.o
	gcc -o app main.o libbcm2836.a common/listlib/liblist.a devices/DS1631/libds1631.a -pthread -lrt

# generate a static library (archive)
#lib: objs ${OBJECTS}
#	ar rcs libbcm2836.a ${OBJECTS}
lib: objs
	ar rcs libbcm2836.a *.o

main.o: main.c
	gcc -c -std=c99 main.c

# remove all object files
clean:
	rm -f *.o

