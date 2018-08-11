#include<glm\glm.hpp>
#include<GL\freeglut.h>
#include<glm/ext.hpp>
#include<malloc.h>
#include<iostream>

using namespace glm;
using namespace std;

typedef struct{
	double x, y, z, h;
} point;

typedef struct{
	int p1, p2, p3;
	double a, b, c, d;
}poly;

dmat4 calcTransf(point ociste, point glediste){
	//matrica t1 pomice ishodiste koor sustava u ociste.

	dmat4 t1 = dmat4(1, 0, 0, -ociste.x,
		0, 1, 0, 0 - ociste.y,
		0, 0, 1, -ociste.z,
		0, 0, 0, 1);



	//matrica t2 rotira z os sustava scene da se poklopi sa sustavom oka za kut alpha 'A'. Odreduju se argumenti matrice sinA i cosA
	double sinA = (glediste.y - ociste.y) / sqrt(pow((glediste.y - ociste.y), 2.0) + pow((glediste.x - ociste.x), 2.0));
	double cosA = (glediste.x - ociste.x) / sqrt(pow((glediste.y - ociste.y), 2.0) + pow(glediste.x - ociste.x, 2.0));



	dmat4 t2 = dmat4(cosA, sinA, 0, 0,
		-sinA, cosA, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);



	//odredivanje pomocnih koef x2 i z2 nakon prve rotacije. Preko njih racunamo sinB i cosB
	double x2 = sqrt(pow(glediste.x - ociste.x, 2.0) + pow(glediste.y - ociste.y, 2.0));
	double z2 = glediste.z - ociste.z;

	double sinB = x2 / sqrt(pow(x2, 2) + pow(z2, 2));
	double cosB = z2 / sqrt(pow(x2, 2) + pow(z2, 2));

	//matrica t3 rotira y os sustava scene da bi se poklopio sa sustavom oka za kut beta 'B'
	dmat4 t3 = dmat4(cosB, 0, -sinB, 0,
		0, 1, 0, 0,
		sinB, 0, cosB, 0,
		0, 0, 0, 1);


	//roacija oko z osi za 90
	dmat4 t4 = dmat4(0, 1, 0, 0,
		-1, 0, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);


	//promjena predznaka x osi
	dmat4 t5 = dmat4(-1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	dmat4 t = t1*t2*t3*t4*t5;


	return t;
}

GLdouble *calcMat(dmat4 m){
	GLdouble *x;
	x = (GLdouble*)malloc(16 * sizeof(GLdouble));
	int k = 0;
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			x[k] = m[i][j];
			if (i == 2) x[k] = -x[k];

			k++;
		}

	}
	return x;
}


dmat4 projection(point ociste, point glediste, poly *poligoni, int brPol){
	//Ucitaj dvije tocke: ociste i glediste. Ociste upisuj dok nije izvan tijela.
	double H = sqrt(pow(ociste.x - glediste.x, 2) + pow(ociste.y - glediste.y, 2) + pow(ociste.z - glediste.z, 2));


	dmat4 pro = dmat4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 0, 0,
		0, 0, 1 / H, 0);

	return pro;

}

int checkPoint(point t, poly *poligoni, int brPol){
	vec3 p = vec3(t.x, t.y, t.z);
	for (int i = 0; i < brPol; i++){
		vec3 pol = vec3(poligoni[i].a, poligoni[i].b, poligoni[i].c);
		if (dot(p, pol) + poligoni[i].d > 0){
			return 0;
		}
	}
	return 1;
}

void sredi(point *vrhovi, int brVrh, dmat4 t, dmat4 pro){
	for (int i = 0; i < brVrh; i++){
		dvec4 temp = dvec4(vrhovi[i].x, vrhovi[i].y, vrhovi[i].z, vrhovi[i].h);
		temp = temp*t*pro;
		vrhovi[i].x = temp.x;
		vrhovi[i].y = temp.y;
		vrhovi[i].z = temp.z;
		vrhovi[i].h = temp.w;

		if (vrhovi[i].h != 1){
			vrhovi[i].x = vrhovi[i].x / vrhovi[i].h;
			vrhovi[i].y = vrhovi[i].y / vrhovi[i].h;
			vrhovi[i].z = vrhovi[i].z / vrhovi[i].h;
			vrhovi[i].h = 1;
		}
	}

}
