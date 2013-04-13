SRCDIR  = src
IDIR    = include
ODIR    = obj
LDIR    = lib
TESTDIR = tests

CC      = clang
CCFLAGS = -I$(IDIR) -Wextra -Wall -x c++
CCFLAGS += `pkg-config --cflags glewmx gl sdl2`

LIBS = `pkg-config --libs glewmx gl sdl2`

_OBJ = bl_sdl.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

MAIN = $(ODIR)/main.o $(OBJ) 

debug: CC += -DDEBUG -g
debug: main

$(ODIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CCFLAGS)

main: $(MAIN)
	$(CC) -o $@ $^ $(CCFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -rf $(ODIR)/*.o 
