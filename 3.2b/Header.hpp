#include<glm\glm.hpp>
#include<GL\freeglut.h>
#include<glm/ext.hpp>
#include<malloc.h>
#include<iostream>
using namespace glm;

typedef struct{
	double x, y, z, h;
} point;

typedef struct{
	int p1, p2, p3;
	double a, b, c, d;
}poly;
GLdouble *calcMat(dmat4 m);
int checkPoint(point t, poly *poligoni, int brPol);
dmat4 calcTransf(point ociste, point glediste);
dmat4 projection(point ociste, point glediste, poly* poligoni, int brPol);
void sredi(point *vrhovi, int brVrh, dmat4 t, dmat4 pro);
