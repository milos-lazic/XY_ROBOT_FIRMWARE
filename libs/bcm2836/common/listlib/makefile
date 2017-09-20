define COMPILE_SOURCE
gcc -c -std=c99 $(1);
endef

# list of source files
SOURCES := sl_list.c

# generate object files
objs:
	$(foreach source, $(SOURCES), $(call COMPILE_SOURCE, $(source)))

# generate a statis library (archive)
lib: objs
	ar rcs liblist.a *.o

# remove all stale object files
clean:
	rm -f *.o