#include<GL\freeglut.h>
#include"Functions.hpp"

void bresenham_draw_whole(GLint xs, GLint ys, GLint xe, GLint ye){
	if (xs <= xe){
		if (ys <= ye){
			bresenham_draw_whole2(xs, ys, xe, ye);
		}
		else{
			bresenham_draw_whole3(xs, ys, xe, ye);
		}
	}
	else{
		if (ys >= ye){
			bresenham_draw_whole2(xe, ye, xs, ys);
		}
		else{
			bresenham_draw_whole3(xe, ye, xs, ys);
		}
	}
}

void bresenham_draw_whole2(GLint xs, GLint ys, GLint xe, GLint ye){
	GLint x, yc, corection;
	GLint a, yf;

	
	if (ye - ys <= xe - xs){
		a = 2 * (ye - ys);
		yc = ys;
		yf = -(xe - xs);
		corection = -2 * (xe - xs);
		for (x = xs; x <= xe; x++){
			glVertex2i(x, yc);
			yf += a;
			if (yf >= 0){
				yf += corection;
				yc += 1;
			}
		}
	}
	else{
		x = xe; xe = ye; ye = x;
		x = xs; xs = ys; ys = x;
		a = 2 * (ye - ys);
		yc = ys; yf = -xe + xs; corection = -2 * (xe - xs);
		for (x = xs; x <= xe; x++){
			glVertex2i(yc, x);
			yf += a;
			if (yf >= 0){
				yf += corection;
				yc += 1;
			}
		}
	}

}


void bresenham_draw_whole3(GLint xs, GLint ys, GLint xe, GLint ye){
	GLint x, yc, corection;
	GLint a, yf;

	
	if (-(ye - ys) <= xe - xs){
		a = 2 * (ye - ys);
		yc = ys;
		yf = xe - xs;
		corection = 2 * (xe - xs);
		for (x = xs; x <= xe; x++){
			glVertex2i(x, yc);
			yf += a;
			if (yf <= 0){
				yf += corection;
				yc -= 1;
			}
		}
	}
	else{
		x = xe; xe = ys; ys = x;
		x = xs; xs = ye; ye = x;
		a = 2 * (ye - ys);
		yc = ys; yf = xe - xs; corection = 2 * (xe - xs);
		for (x = xs; x <= xe; x++){
			glVertex2i(yc,x);
			yf += a;
			if (yf <= 0){
				yf += corection;
				yc -= 1;
			}
		}
	}

}