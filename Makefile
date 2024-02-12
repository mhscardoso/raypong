CFLAGS=-Wall -Wextra
DEPS=-lraylib -lgdi32 -lopengl32 -lwinmm
INCLUDE=-I raylib/src/
LIB=-L raylib/src/

run:main
	.\game

main:game/main.cpp
	g++ game/main.cpp -o game \
	$(CFLAGS) \
	$(INCLUDE) \
	$(LIB) \
	$(DEPS)