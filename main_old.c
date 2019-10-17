#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libgfxppm/gfx.h"

#define _FOV 0.70

void drawLine(struct gfx_PPMStruct* PPMStruct, int x1, int y1, int x2, int y2){
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
		gfx_PPMSetPixel(x, y, PPMStruct, 0xFF, 0xFF, 0xFF);

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
			gfx_PPMSetPixel(x, y, PPMStruct, 0xFF, 0xFF, 0xFF);
		}
	}else{
		if(dy>=0){
			x=x1;y=y1;ye=y2;
		}else{
			x=x2;y=y2;ye=y1;
		}

		gfx_PPMSetPixel(x, y, PPMStruct, 0xFF, 0xFF, 0xFF);
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
			gfx_PPMSetPixel(x, y, PPMStruct, 0xFF, 0xFF, 0xFF);
		}
	}
}

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
	struct gfx_PPMStruct* MyPPMStructure = gfx_PPMSetup(1000, 1000, 0, 0, 0);
	
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

	// rotate by 45 degrees
	float rad = 3.14/3;
	printf("Radian: %f\n", rad);
	for(int i = 0; i<8; i++){
		xM[i] = xM[i] * cos(rad) - zM[i] * sin(rad);
		//yM[i] = xM[i] * sin(rad45) + yM[i] * cos(rad45);
		zM[i] = xM[i] * sin(rad) + zM[i] * cos(rad);
		//zM[i] = zM[i];
	}

	// draw points
	for(int i = 0; i<8; i++){
		gfx_PPMSetPixel(
				tra(convert3Dto2D(xM[i], zM[i])),
				tra(convert3Dto2D(yM[i], zM[i])),
				MyPPMStructure,
				0xFF, 0xFF, 0xFF);
	}

	// static drawing of lines
	void drawByPoints(int i, int j){
		drawLine(MyPPMStructure, tra(convert3Dto2D(xM[i], zM[i])), tra(convert3Dto2D(yM[i], zM[i])), tra(convert3Dto2D(xM[j], zM[j])), tra(convert3Dto2D(yM[j], zM[j])));
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

	gfx_PPMExport(MyPPMStructure, "output.ppm");
	return 0;
}
