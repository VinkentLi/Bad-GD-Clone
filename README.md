# Bad-GD-Clone

Clone of Geometry Dash 1.0

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

Run these commands to build for the web
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
- A lot of things, none of which are worth doing
