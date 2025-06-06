# Bad-GD-Clone

Clone of Geometry Dash 1.0

Play a webport here at [https://vinkent.li/gd/](https://vinkent.li/gd/)

## How to build

1. If you're on Linux or MacOS, use this [Installation Guide by SDL2](https://wiki.libsdl.org/SDL2/Installation)

2. Otherwise, use these links to download SDL2 versions (make sure you download the files with "devel" in them): [SDL2](https://github.com/libsdl-org/SDL/releases/tag/release-2.32.8), [SDL2_image](https://github.com/libsdl-org/SDL_image/releases/tag/release-2.8.8), [SDL2_mixer](https://github.com/libsdl-org/SDL_mixer/releases/tag/release-2.8.1), [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.24.0)

3. Then, save the folders inside of them somewhere. When you run the third command below, you will need to include this at the end: `-DSDL2_DIR=path/to/sdl2/cmake -DSDL2_image_DIR=path/to/sdl2_image/cmake -DSDL2_mixer_DIR=path/to/sdl2_mixer/cmake -DSDL2_ttf_DIR=path/to/sdl2_ttf/cmake`

4. Run these commands to build and run

```bash
mkdir build
cd build
cmake -S ../ -DCMAKE_BUILD_TYPE=Release
cmake --build .
cd ..
./build/GDClone
```

### Building for web

1. [Install Emscripten](https://emscripten.org/docs/getting_started/downloads.html)
2. Run these commands to build for the web (you do not need to set the SDL2 paths for this)
```bash
mkdir build-web
cd build-web
emcmake cmake -S ../ -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

3. Then use this html code

```html
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8" />
</head>
<body>
    <canvas style="height: 80vh; overflow: hidden; display: block; margin: 0 auto;" id="canvas" oncontextmenu="event.preventDefault()"></canvas>
    <script>
        var Module = {
            canvas: (function() { return document.getElementById('canvas'); })()
        };
    </script>
    <script src="GDClone.js"></script>
</body>
</html>
```

## TODO
- Add more buttons to make it more mobile friendly
- A lot of other things, none of which are worth doing
