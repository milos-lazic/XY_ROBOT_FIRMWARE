define COMPILE_SOURCE
gcc -c -std=c99 $(1);
endef

# list of source files
SOURCES := DS1631.c

# generate object files
objs:
	$(foreach source, $(SOURCES), $(call COMPILE_SOURCE, $(source)))

# generate a static library (archive)
lib: objs
	ar rcs libds1631.a *.o ../../libbcm2836.a

# remove all stale object files
clean:
	rm -f *.o