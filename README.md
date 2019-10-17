# RotatingBoxInFramebuffer
Experiment on rotating 3D box in 2D space

# READ ME!
This experiment took me 2 days, and there is a lot of spaghetti code. I may clean that up later since i didn't have any library for framebuffer drawing. Also there is error somewhere in angle calculations, becouse after some time box becomes X=0, Y=100, Z=100, or something like that, making it single line of pixels.

Also! 3D to 2D conversion algorithm is creation of my mind, on how this should work (and as it turns out, actually do). Don't take it seriously if you think on making something *quality*.

## Building for ppm images
```
git clone https://github.com/d3suu/RotatingBoxInFramebuffer
cd RotatingBoxInFramebuffer
git clone https://github.com/d3suu/libgfxppm
make libgfx
./libgfx
feh output.ppm
```

## Sites which helped me while making and learning

 - https://u.osu.edu/cstutorials/2018/09/28/how-to-debug-c-program-using-gdb-in-6-simple-steps/
 - http://smurf.mimuw.edu.pl/external_slides/Przeksztalcenia_geometryczne/Przeksztalcenia_geometryczne.html
 - https://www.freecodecamp.org/news/how-to-code-your-first-algorithm-draw-a-line-ca121f9a1395/
