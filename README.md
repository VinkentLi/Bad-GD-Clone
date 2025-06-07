# Bad-GD-Clone

Clone of Geometry Dash 1.0

Play a webport here at [https://vinkent.li/gd/](https://vinkent.li/gd/)

## How to build

Run these commands to build (it will install SDL2 for you if you don't have it)

```bash
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build/ --parallel
```

The game will be in the build folder (on MSVC you might have to go inside a `Release` folder). You will either have to copy the `res` folder in manually or just run `./build/GDClone` from the source directory

### Building for web

1. [Install Emscripten](https://emscripten.org/docs/getting_started/downloads.html)
2. Run these commands to build for the web (you do not need to set the SDL2 paths for this)
```bash
mkdir build-web
cd build-web
emcmake cmake -S ../ -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
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
