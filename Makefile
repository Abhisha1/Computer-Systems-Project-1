CC     = gcc
CFLAGS = -Wall -std=c99 -g
# modify the flags here ^
EXE    = image_tagger
OBJ    = hashtable.o http-parser.o http-response.o http-server.o
# add any new object files here ^
# top (default) target
all: $(EXE)

# how to link executable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) -lm

# other dependencies
http-parser.o: http-parser.h hashtable.h
http-response.o: http-response.h http-parser.h hashtable.h
http-server.o: http-parser.h

# ^ add any new dependencies here (for example if you add new modules)


# phony targets (these targets do not represent actual files)
.PHONY: clean cleanly all CLEAN

# `make clean` to remove all object files
# `make CLEAN` to remove all object and executable files
# `make cleanly` to `make` then immediately remove object files (inefficient)
clean:
	rm -f $(OBJ)
CLEAN: clean
	rm -f $(EXE)
cleanly: all clean