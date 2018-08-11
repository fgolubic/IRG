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

complex c;

void renderScene();
int divergence_test(complex z0, complex c, int limit, int epsilonSquare);
void display();
void reshape(int width, int height);
int max(int a, int b);

int main(int argc, char** args){

	cout << "Upisi eps i maksimalni broj iteracija:" << endl;
	cin >> eps >> m;

	cout << "Upisi c.re i c.im" << endl;
	cin >> c.re >> c.im;

	umax = 1; umin = -2;
	vmin = -1.2; vmax = 1.2;

	glutInit(&argc, args);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);

	glutCreateWindow("Julij");

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

int divergence_test(complex z0, complex c, int limit, int epsilonSquare) {
	 complex z;
	 z.re = z0.re; z.im = z0.im;
	 double modul2 = z.re *z.re + z.im*z.im;
	 if(modul2 > epsilonSquare) return 0;
	 for (int i = 1; i <= limit; i++) {
		 double next_re = z.re *z.re - z.im* z.im + c.re;
		 double next_im = 2* z.re *z.im + c.im;
		 z.re = next_re;
		 z.im = next_im;
		 double modul2 = z.re * z.re + z.im* z.im;
		 if(modul2 > epsilonSquare) return i;
		
	}
	 return -1;
	
}

 int max(int a, int b) {
	 return (b>a) ? b : a;
	
}

 void renderScene() {
	// int limit = 64;
	
	 // Ovdje j e odabran parametar ’ c ’ :
	//	 c.re = 0.32; c.im = 0.43;
	 double epsilon = max(c.re *c.re + c.im* c.im, 2.0);
	 int epsilonSquare = epsilon * epsilon;
	 int limit = m;
	 glPointSize(1);
	 glBegin(GL_POINTS);
	 for (int y = 0; y < h; y++) {
		 for (int x =0; x <w; x++) {
			 complex z0;
			 z0.re = (x) / (double)w*(umax-umin) + umin;
			 z0.im = (y) / (double)h*(vmax-vmin) + vmin;
			 int n = divergence_test(z0, c, limit, epsilonSquare);
			 if(n == -1) {
				  glColor3f(0.0f, 0.0f, 0.0f);
				 
			 } else{
				  glColor3f((double)n / limit,
				  1-(double) n / limit / 2.0,
				  0.8-(double) n / limit / 3.0);
				  }
			  glVertex2i(x, h-y);
			 
		 }
		 
	 }
	  glEnd();
	  glutSwapBuffers();
 }
