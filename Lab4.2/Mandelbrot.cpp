#include<iostream>
#include<stdio.h>
#include<glm\glm.hpp>
#include<GL\freeglut.h>

using namespace std;

GLint w, h;
int umax, umin, vmax, vmin;
int eps;
int m;
int xmax, ymax, ymin, xmin;

typedef struct {
	 double re;
	 double im;
	
} complex;

void renderScene();
int divergence_test(complex c, int limit);
void display();
void reshape(int width, int height);

int main(int argc, char** args){

	cout << "Upisi eps i maksimalni broj iteracija:" << endl;
	cin >> eps >> m;
	
	//cout << "Upisi u i v max i min" << endl;
	//cin >> umax >> umin >> vmax >> vmin;

	umax = 0,5; umin = -1.5;
	vmin = -1; vmax = 1;

	glutInit(&argc, args);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);

	glutCreateWindow("Mandelbrot");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutMainLoop();

}



void display(){

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	
	renderScene();
}





void reshape(int width, int height){
	w = width;
	h = height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width - 1, 0, height - 1);
	glMatrixMode(GL_MODELVIEW);

}

int divergence_test(complex c, int limit) {
	complex z;
	z.re = 0; z.im = 0;
	for (int i = 1; i <= limit; i++) {
		double next_re = z.re *z.re - z.im* z.im + c.re;
		double next_im = 2* z.re *z.im + c.im;
		z.re = next_re;
		z.im = next_im;
		double modul2 = z.re * z.re + z.im* z.im;
		if(modul2 > eps*eps) return i;

	}
	return -1;
}


void renderScene() {
	 glPointSize(1);
	 glBegin(GL_POINTS);
	 for (int y = 0; y < h; y++) {
		 for (int x = 0; x <w; x++) {
			 complex c;
			 c.re = (x) / (double)w *(umax-umin) + umin;
			 c.im = (y) / (double)h *(vmax-vmin) + vmin;
			 int n = divergence_test(c, m);
			 if(n == -1) {
				 glColor3f(0.0f, 0.0f, 0.0f);
				
			} else{
				 glColor3f((double)n / m,
					  1.0-(double) n / m / 2.0,
					  0.8-(double) n / m / 3.0);
				 
				 }
			 glVertex2i(x, h-y);
			
		}
		
	}
	 glEnd();
	 glutSwapBuffers();
}