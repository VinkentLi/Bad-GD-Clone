# Bad-GD-Clone

The name speaks for itself. Clone of GD 1.0 written in my terrible C++ code.

## How to build

Run these commands to build and run

```bash
mkdir build
cd build
cmake -S ../
cmake --build .
cd ..
./build/GDClone
```

Run these commands to build for the web (note: building for web is currently not working)
```bash
mkdir build-web
cd build-web
emcmake cmake -S ../
cmake --build .
```

Then use this html code

```html
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8" />
</head>
<body>
    <canvas class="relative w-2/5" style="height: 50vh; overflow: hidden;" id="canvas" oncontextmenu="event.preventDefault()"></canvas>
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
- Make ship rotation more accurate
- Make it read from the actual game files?
- Tidy up code
