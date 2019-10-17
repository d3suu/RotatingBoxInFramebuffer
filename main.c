#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "libgfxppm/gfx.h"

#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define _FOV 0.70

int convert3Dto2D(int in, int z){
	if(z==0){
		return in;
	}else if(z>0){
		//return (sqrt((float)z)*(float)in);
		float i = (float)z;
		while(i>_FOV)
			i -= 0.1;
		return i*in;
	}else if(z<0){
		float i = (float)z;
		while(i<_FOV)
			i += 0.1;
		return i*in;
	}
}

int tra(int a){ // translate, move by vector (x,y)+200 for PPMStructure which starts from 0,0
	return a+200;
}

int main(){
	int fbfd = 0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long int screensize = 0;
	char *fbp = 0;
	long int location = 0;
	
	// Open the file for reading and writing
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd == -1){
		perror("Error: cannot open framebuffer device");
		exit(1);
	}
	printf("The framebuffer device was opened successfully.\n");
	// Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1){
		perror("Error reading fixed information");
		exit(2);
	}
	// Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1){
		perror("Error reading variable information");
		exit(3);
	}
	printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
	// Map the device to memory
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if ((int)fbp == -1){
		perror("Error: failed to map framebuffer device to memory");
		exit(4);
	}
	printf("The framebuffer device was mapped to memory\n");

	void drawPixel(int x, int y){
		location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;
		if (vinfo.bits_per_pixel == 32) {
			*(fbp + location) = 0xFF; // Blue
			*(fbp + location + 1) = 0xFF; // Green
			*(fbp + location + 2) = 0xFF; // Red
			*(fbp + location + 3) = 0; // No transparency
		}
	}
	
	void drawPixelBlack(int x, int y){
		location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;
		if (vinfo.bits_per_pixel == 32) {
			*(fbp + location) = 0x00; // Blue
			*(fbp + location + 1) = 0x00; // Green
			*(fbp + location + 2) = 0x00; // Red
			*(fbp + location + 3) = 0; // No transparency
		}
	}
	
	void drawLine(int x1, int y1, int x2, int y2){
		// This is from https://www.freecodecamp.org/news/how-to-code-your-first-algorithm-draw-a-line-ca121f9a1395/
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		
		dx = x2-x1;
		dy = y2-y1;
		dx1 = abs(dx);
		dy1 = abs(dy);
		px = 2*dy1-dx1;
		py = 2*dx1-dy1;

		if(dy1<=dx1){
			if(dx >= 0){
				x = x1; y = y1; xe = x2;
			}else{
				x = x2; y = y2; xe = x1;
			}
			//gfx_PPMSetPixel(x, y, PPMStruct, 0xFF, 0xFF, 0xFF);
			drawPixel(x, y);

			for(i=0; x<xe; i++){
				x=x+1;
				if(px<0){
					px=px+2*dy1;
				}else{
					if((dx<0 && dy<0) || (dx>0 && dy>0)){
						y=y+1;
					}else{
						y=y-1;
					}
					px = px+2*(dy1-dx1);
				}
				//gfx_PPMSetPixel(x, y, PPMStruct, 0xFF, 0xFF, 0xFF);
				drawPixel(x, y);
			}
		}else{
			if(dy>=0){
				x=x1;y=y1;ye=y2;
			}else{
				x=x2;y=y2;ye=y1;
			}

			//gfx_PPMSetPixel(x, y, PPMStruct, 0xFF, 0xFF, 0xFF);
			drawPixel(x, y);
			for(i=0; y<ye; i++){
				y=y+1;
				if(py<=0){
					py=py+2*dx1;
				}else{
					if((dx<0 && dy<0) || (dx>0 && dy>0)){
						x=x+1;
					}else{
						x=x-1;
					}
					py=py+2*(dx1-dy1);
				}
				//gfx_PPMSetPixel(x, y, PPMStruct, 0xFF, 0xFF, 0xFF);
				drawPixel(x, y);
			}
		}
	}


	
	/* Points:
	 * ID	x	y	z
	 * 1	100	100	0
	 * 2	100	-100	0
	 * 3	-100	-100	0
	 * 4	-100	100	0
	 * 5	100	100	2
	 * 6	100	-100	2
	 * 7	-100	-100	2
	 * 8	-100	100	2
	 */

	int xM[8] = {100,  100, -100, -100, 100,  100, -100, -100};
	int yM[8] = {100, -100, -100,  100, 100, -100, -100,  100};
	int zM[8] = {  0,    0,    0,    0, 100,  100,  100,  100};
// LOOP HERE -------------------------------------------------------------------
for(float rad = 0.00; rad<6.28; rad += 0.01){
	// CLEAR SCREEN
	for(int aA = 100; aA<300; aA++){
		for(int aB = 100; aB<300; aB++){
			drawPixelBlack(aA, aB);
		}
	}
	// rotate by 45 degrees
	//float rad = 3.14/3;
	//printf("Radian: %f\n", rad);
	for(int i = 0; i<8; i++){
		xM[i] = xM[i] * cos(rad) - zM[i] * sin(rad);
		//yM[i] = xM[i] * sin(rad45) + yM[i] * cos(rad45);
		zM[i] = xM[i] * sin(rad) + zM[i] * cos(rad);
		//zM[i] = zM[i];
	}

	// draw points
	for(int i = 0; i<8; i++){
		/*
		gfx_PPMSetPixel(
				tra(convert3Dto2D(xM[i], zM[i])),
				tra(convert3Dto2D(yM[i], zM[i])),
				MyPPMStructure,
				0xFF, 0xFF, 0xFF);
				*/
		drawPixel(
				tra(convert3Dto2D(xM[i], zM[i])),
				tra(convert3Dto2D(yM[i], zM[i]))
			 );	
	}

	// static drawing of lines
	void drawByPoints(int i, int j){
		drawLine(tra(convert3Dto2D(xM[i], zM[i])), tra(convert3Dto2D(yM[i], zM[i])), tra(convert3Dto2D(xM[j], zM[j])), tra(convert3Dto2D(yM[j], zM[j])));
	}

	drawByPoints(0, 1);
	drawByPoints(0, 3);
	drawByPoints(2, 1);
	drawByPoints(2, 3);
	
	drawByPoints(4, 5);
	drawByPoints(4, 7);
	drawByPoints(6, 5);
	drawByPoints(6, 7);
	
	drawByPoints(7, 3);
	drawByPoints(2, 6);
	drawByPoints(1, 5);
	drawByPoints(4, 0);

	sleep(1);
	if(rad > 3.14)
		rad = 0.00;
} // END OF LOOP HERE ---------------------------------------
	munmap(fbp, screensize);
	close(fbfd);
	return 0;
}
