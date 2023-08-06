debug:
	g++ src/*.cpp -o bin/debug/game -g -O0 -Wall -I include -I "C:/SDL2-w64/include" -L "C:/SDL2-w64/lib" -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

release:
	g++ src/*.cpp -o bin/release/game -O2 -Wall -I include -I "C:/SDL2-w64/include" -L "C:/SDL2-w64/lib" -s -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

all:
	debug
