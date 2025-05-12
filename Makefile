# Source and include setup
SRC := $(wildcard src/*.cpp)
INCLUDES := -I include

# Output paths
DEBUG_OUT := bin/debug/game.exe
RELEASE_OUT := bin/release/game.exe
WEB_OUT := bin/web/index.js

# SDL2 paths for Windows (native builds)
SDL_WIN_INC := -I "C:/SDL2-w64/include"
SDL_WIN_LIB := -L "C:/SDL2-w64/lib"
SDL_WIN_LIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# SDL2 flags for Web (Emscripten builds)
SDL_WEB_LIBS := -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"] \
                -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2

# Global compiler flags
CXXFLAGS := -Wall $(INCLUDES)

.PHONY: all debug release web clean

# Default target
all: debug

# Native Windows builds
debug:
	g++ $(SRC) -o $(DEBUG_OUT) -g -O0 $(CXXFLAGS) $(SDL_WIN_INC) $(SDL_WIN_LIB) $(SDL_WIN_LIBS)

release:
	g++ $(SRC) -o $(RELEASE_OUT) -O2 -s -mwindows $(CXXFLAGS) $(SDL_WIN_INC) $(SDL_WIN_LIB) $(SDL_WIN_LIBS)

# WebAssembly build using emscripten
web:
	em++ $(SRC) -o $(WEB_OUT) -O2 $(CXXFLAGS) $(SDL_WEB_LIBS) -s WASM=1 -s ALLOW_MEMORY_GROWTH -s ASSERTIONS --preload-file res

# Clean build artifacts
clean:
	del /Q bin\\debug\\* bin\\release\\* bin\\web\\* 2> NUL || true
