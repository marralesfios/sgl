COMPOPT := -std=c++26 -flto=7 -fuse-linker-plugin -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -m64 -O3 -I"src/include" -I"/usr/include/freetype2" -I$(cppinclude)
FINALOPT := $(COMPOPT) -s -L$(cpplibs) -lSDL3 -lglad -lfreetype -lfontconfig -lstbimage -lharfbuzz
LIBNAME := sgl
HEADERS := $(wildcard src/include/$(LIBNAME)/*.hpp) $(wildcard src/include/$(LIBNAME)/*/*.hpp)
SOURCES := $(wildcard src/implementation/*.cpp) $(wildcard src/implementation/*/*.cpp)
OBJECTS := $(patsubst src/implementation/%.cpp,obj/%.o,$(SOURCES))
bin/%: src/%.cpp lib/$(LIBNAME).a
	g++ $< lib/$(LIBNAME).a -o $@ $(FINALOPT)
lib/$(LIBNAME).a: $(OBJECTS)
	ar rcs $@ $(OBJECTS)
obj/%.o: src/implementation/%.cpp $(HEADERS)
	g++ -c $< -o $@ $(COMPOPT)
setup:
	mkdir lib
	mkdir bin
	mkdir obj
	mkdir obj/draw
	mkdir obj/ext
.PHONY: setup
	