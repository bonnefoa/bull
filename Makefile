SRCDIR  = src
IDIR    = include
ODIR    = obj
LDIR    = lib
TESTDIR = tests

CC      = clang++
CCFLAGS = -I$(IDIR) -Wextra -Wall -Werror
CCFLAGS += `pkg-config --cflags glew gl sdl2 bullet assimp libpng yaml-cpp SDL2_ttf SDL_net`

LIBS = `pkg-config --libs glew gl sdl2 bullet assimp libpng yaml-cpp SDL2_ttf SDL_net`

TEST_CCFLAGS = $(CCFLAGS)
TEST_CCFLAGS += `pkg-config --cflags check`
TEST_LIBS = $(LIBS)
TEST_LIBS += `pkg-config --libs check`

_SRCS = $(wildcard $(SRCDIR)/*.cpp)
SRCS = $(filter-out %/main.cpp, $(_SRCS))

_OBJ = $(patsubst $(SRCDIR)/%,$(ODIR)/%,$(SRCS))
OBJ = $(patsubst %.cpp,%.o,$(_OBJ))

MAIN = $(ODIR)/main.o $(OBJ) 

debug: CC += -DDEBUG -g
debug: main

$(ODIR)/%.d: $(SRCDIR)/%.cpp
	$(CC) $(CCFLAGS) -MM -MG $< | sed -e 's,^\([^:]*\)\.o[ ]*:,$(@D)/\1.o $(@D)/\1.d:,' >$@

include $(SRCS:$(SRCDIR)/%.cpp=$(ODIR)/%.d)

%(ODIR)/%.d: $(SRCDIR)/%.cpp

$(ODIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) -c -o $@ $<

$(ODIR)/%.o: $(TESTDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) -c -o $@ $<

check_%: $(ODIR)/check_%.o $(OBJ)
	$(CC) $(TEST_CCFLAGS) $(TEST_LIBS) -o $@ $^

main: $(MAIN)
	$(CC) $(CCFLAGS) $(LIBS) -o $@ $^ 

.PHONY: clean debug

clean:
	rm -f $(ODIR)/*
	rm -f check_*
