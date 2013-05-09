SRCDIR  = src
IDIR    = include
ODIR    = obj
LDIR    = lib
TESTDIR = tests

CC      = g++
CCFLAGS = -I$(IDIR) -Wextra -Wall
CCFLAGS += `pkg-config --cflags glew gl sdl2 bullet assimp libxml-2.0`
CCFLAGS += -I/usr/include/opencollada/COLLADAFramework/ -I/usr/include/opencollada/COLLADABaseUtils/ -I/usr/include/opencollada/COLLADASaxFrameworkLoader/

LIBS = `pkg-config --libs glew gl sdl2 bullet assimp libxml-2.0 libpcre`
LIBS += -L/usr/lib64/opencollada 
LIBS += -lOpenCOLLADAFramework
LIBS += -lOpenCOLLADASaxFrameworkLoader
#LIBS += -lGeneratedSaxParser
#LIBS += -lOpenCOLLADABaseUtils
#LIBS += -lOpenCOLLADAStreamWriter
#LIBS += -lMathMLSolver -lftoa -lbuffer -lUTF

TEST_CCFLAGS = $(CCFLAGS)
TEST_CCFLAGS += `pkg-config --cflags check`
TEST_LIBS = $(LIBS)
TEST_LIBS += `pkg-config --libs check`

_OBJ = bl_window.o bl_input.o bl_simulation.o bl_file.o bl_shader.o bl_model.o bl_program.o bl_util.o bl_program_model.o bl_log.o bl_loader.o bl_collada.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

MAIN = $(ODIR)/main.o $(OBJ) 

_TESTS = check_simulation.o check_model.o
TESTS = $(patsubst %,$(ODIR)/%,$(_TESTS))

debug: CC += -DDEBUG -g
debug: main

$(ODIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CCFLAGS)

$(ODIR)/%.o: $(TESTDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CCFLAGS)

check_simulation: $(ODIR)/check_simulation.o $(OBJ) 
	$(CC) -o $@ $^ $(TEST_CCFLAGS) $(TEST_LIBS)

check_model: $(ODIR)/check_model.o $(OBJ) 
	$(CC) -o $@ $^ $(TEST_CCFLAGS) $(TEST_LIBS)

check_collada: $(ODIR)/check_collada.o $(OBJ) 
	$(CC) -o $@ $^ $(TEST_CCFLAGS) $(TEST_LIBS)

check_input: $(ODIR)/check_input.o $(OBJ) 
	$(CC) -o $@ $^ $(TEST_CCFLAGS) $(TEST_LIBS)

main: $(MAIN)
	$(CC) -o $@ $^ $(CCFLAGS) $(LIBS)

.PHONY: clean check_input check_model check_collada

clean:
	rm -rf $(ODIR)/*.o 
	rm check_*
