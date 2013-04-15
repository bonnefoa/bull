SRCDIR  = src
IDIR    = include
ODIR    = obj
LDIR    = lib
TESTDIR = tests

CC      = clang
CCFLAGS = -I$(IDIR) -Wextra -Wall -x c++
CCFLAGS += `pkg-config --cflags glewmx gl sdl2 bullet`

LIBS = `pkg-config --libs glewmx gl sdl2 bullet`

_OBJ = bl_window.o bl_input.o bl_render.o bl_simulation.o bl_file.o bl_shader.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

MAIN = $(ODIR)/main.o $(OBJ) 

_TESTS = check_simulation.o
TESTS = $(patsubst %,$(ODIR)/%,$(_TESTS))

debug: CC += -DDEBUG -g
debug: main

$(ODIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CCFLAGS)

$(ODIR)/%.o: $(TESTDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CCFLAGS)

check: $(TESTS) $(OBJ) 
	$(CC) -o $@ $^ $(CCFLAGS) $(LIBS) -lcheck
	./check

main: $(MAIN)
	$(CC) -o $@ $^ $(CCFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -rf $(ODIR)/*.o 
