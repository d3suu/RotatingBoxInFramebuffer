test: main.c
	gcc main.c -g -lm -o test

libgfx: main_old.c
	gcc main.c libgfxppm/gfx.* -g -lm -o libgfx

fast: main.c
	gcc main.c -O3 -o fast

clear:
	rm test libgfx fast
