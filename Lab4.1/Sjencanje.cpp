#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <math.h>
#include<iostream>
#define MAX 10000

using namespace std;
using namespace glm;

typedef struct {
	float a;
	float b;
	float c;
} Normal;

typedef struct {
	float x;
	float y;
	float z;
	Normal n;
} Vrh;

typedef struct {
	Vrh *v1;
	Vrh *v2;
	Vrh *v3;
	Normal n;
	bool front;
} Poligon;

GLint width = 500, height = 500;

vector<Vrh> vertices;			// tocke tijela (koje se iscrtavaju na ekran)
vector<Poligon> polygons;			// poligoni tijela
vector<Vrh> original_vertices;	// tocke tijela (one izvorne, iz .obj)

Vrh o;	// ociste
Vrh original_o;
Vrh g;	// glediste
Vrh original_g;
Vrh light;
float H;	// ravnina projekcije

float xmax = -MAX, xmin = MAX, ymax = -MAX, ymin = MAX, zmax = -MAX, zmin = MAX;

float right_ = -MAX, left_ = MAX, top_ = -MAX, bottom_ = MAX;	// za glOrtho

void load();
void myDisplay();
void myReshape(int w, int h);
void myMouse(int button, int state, int x, int y);

void transformation();
void projection();
void frontOrBack();
void normalizeObject();
void vertexNormals();
float constantIntensity(Poligon &p);
void constantShading();
float gouraudIntensity(Vrh &v);
void gouraudShading();

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(150, 150);

	cout << "Upisi koordinate ocista: " << endl;
	scanf("%f %f %f", &original_o.x, &original_o.y, &original_o.z);
	o.x = original_o.x;
	o.y = original_o.y;
	o.z = original_o.z;

	cout << "Upisi koordinate gledista: " << endl;
	scanf("%f %f %f", &original_g.x, &original_g.y, &original_g.z);

	load();
	//frontOrBack();
	//vertexNormals();

	normalizeObject();
	// frontOrBack();
	transformation();
	frontOrBack();
	vertexNormals();
	projection();

	cout << "Upisi koordinate izvora: " << endl;
	scanf("%f %f %f", &light.x, &light.y, &light.z);


	glutCreateWindow("Sjencanje");
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutMouseFunc(myMouse);

	glutMainLoop();

	return 0;
}

void load() {
	ifstream file("C:/Users/Filip/Desktop/kocka.obj");
	string line;
	while (getline(file, line)) {
		if (line[0] == 'v') {
			Vrh v;
			stringstream ss;
			line = line.substr(2);
			ss.str(line);
			ss >> v.x;
			ss >> v.y;
			ss >> v.z;
			original_vertices.push_back(v);
			vertices.push_back(v);
		}
		if (line[0] == 'f') {
			int i;
			Poligon p;
			stringstream ss;
			line = line.substr(2);
			ss.str(line);
			ss >> i;
			p.v1 = &vertices[i - 1];
			ss >> i;
			p.v2 = &vertices[i - 1];
			ss >> i;
			p.v3 = &vertices[i - 1];
			p.front = false;
			polygons.push_back(p);
		}
	}
}

void myDisplay() {
	glClearColor(1, 1, 1, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void myReshape(int w, int h) {
	width = w;
	height = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left_ - 1, right_ + 1, bottom_ - 1, top_ + 1, 20, -20);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, width, height);
}

void myMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		myDisplay();
		constantShading();
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		myDisplay();
		gouraudShading();
	}
}

void transformation() {
	vec4 g_transformed;
	float sin;
	float cos;

	vertices.clear();		// obrisi sve transformacije dosad


	mat4x4 T1 = mat4x4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-o.x, -o.y, -o.z, 1);

	g_transformed = T1 * vec4(original_g.x, original_g.y, original_g.z, 1);

	g.x = g_transformed[0];
	g.y = g_transformed[1];
	g.z = g_transformed[2];

	sin = g.y / sqrt(pow(g.x, 2) + pow(g.y, 2));
	cos = g.x / sqrt(pow(g.x, 2) + pow(g.y, 2));

	mat4x4 T2 = mat4x4(cos, -sin, 0, 0,
		sin, cos, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	g_transformed = T2 * vec4(g.x, g.y, g.z, 1);
	g.x = g_transformed[0];
	g.y = g_transformed[1];
	g.z = g_transformed[2];

	sin = g.x / sqrt(pow(g.x, 2) + pow(g.z, 2));
	cos = g.z / sqrt(pow(g.x, 2) + pow(g.z, 2));

	mat4x4 T3 = mat4x4(cos, 0, sin, 0,
		0, 1, 0, 0,
		-sin, 0, cos, 0,
		0, 0, 0, 1);

	mat4x4 T4 = mat4x4(0, -1, 0, 0,
		1, 0, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	mat4x4 T5 = mat4x4(-1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	for (auto &v : original_vertices) {
		vec4 v_transformed = T5 * T4 * T3 * T2 * T1 * vec4(v.x, v.y, v.z, 1);
		Vrh vt;
		vt.x = v_transformed[0];
		vt.y = v_transformed[1];
		vt.z = v_transformed[2];
		vertices.push_back(vt);
	}

	g_transformed = T5 * T4 * T3 * vec4(g.x, g.y, g.z, 1);
	g.x = g_transformed[0];
	g.y = g_transformed[1];
	g.z = g_transformed[2];

	vec4 o_transformed = T5 * T4 * T3 * T2 * T1 * vec4(o.x, o.y, o.z, 1);
	o.x = o_transformed[0];
	o.y = o_transformed[1];
	o.z = o_transformed[2];
}

void projection() {
	H = sqrt(pow(o.x - g.x, 2) + pow(o.y - g.y, 2) + pow(o.z - g.z, 2));

	for (auto &v : vertices) {
		v.x = (v.x*H) / v.z;
		v.y = (v.y*H) / v.z;
		v.z = H;

		if (left_>v.x) left_ = v.x;
		if (right_<v.x) right_ = v.x;
		if (bottom_>v.y) bottom_ = v.y;
		if (top_<v.y) top_ = v.y;
	}
}

void frontOrBack() {
	for (auto &p : polygons) {
		float a = (p.v2->y - p.v1->y)*(p.v3->z - p.v1->z) - (p.v2->z - p.v1->z)*(p.v3->y - p.v1->y);		// A = (y2-y1)(z3-z1) - (z2-z1)(y3-y1)
		float b = (p.v2->z - p.v1->z)*(p.v3->x - p.v1->x) - (p.v2->x - p.v1->x)*(p.v3->z - p.v1->z);		// B = (z2-z1)(x3-x1) - (x2-x1)(z3-z1)
		float c = (p.v2->x - p.v1->x)*(p.v3->y - p.v1->y) - (p.v2->y - p.v1->y)*(p.v3->x - p.v1->x);		// C = (x2-x1)(y3-y1) - (y2-y1)(x3-x1)
		vec3 n = normalize(vec3(a, b, c));		//racunanje normale poligona
		p.n.a = n[0];
		p.n.b = n[1];
		p.n.c = n[2];
	//	cout << p.n.a <<" "<< p.n.b <<" "<< p.n.c << endl;
		a = (p.v1->x + p.v2->x + p.v3->x) / 3;		//racunanje tezista
		b = (p.v1->y + p.v2->y + p.v3->y) / 3;
		c = (p.v1->z + p.v2->z + p.v3->z) / 3;
		vec3 np = normalize(vec3(a, b, c) -  vec3(o.x, o.y, o.z)); //vektor od ocista do tezista
		if (dot(n, np) > 0) p.front = true;
	}
}

//smjesti tijelo u radni prostor
void normalizeObject() {
	for (auto &v : vertices) {
		if (xmin>v.x) xmin = v.x;
		if (xmax<v.x) xmax = v.x;
		if (ymin>v.y) ymin = v.y;
		if (ymax<v.y) ymax = v.y;
		if (zmin>v.z) zmin = v.z;
		if (zmax<v.z) zmax = v.z;
	}

	float max_range = xmax - xmin;
	if (ymax - ymin > max_range) {
		max_range = ymax - ymin;
	}
	if (zmax - zmin > max_range) {
		max_range = zmax - zmin;
	}

	float center_x = (xmax + xmin) / 2;
	float center_y = (ymax + ymin) / 2;
	float center_z = (zmax + zmin) / 2;

	for (auto &v : original_vertices) {
		v.x = v.x - center_x;
		v.y = v.y - center_y;
		v.z = v.z - center_z;

		v.x = (2 / max_range) * v.x;
		v.y = (2 / max_range) * v.y;
		v.z = (2 / max_range) * v.z;
	}
}


//odredi normale kroz vrhove poligona
void vertexNormals() {
	vector<Poligon> incidentPolygons;
	for (auto &v : vertices) {
		incidentPolygons.clear();
		for (auto &p : polygons) {
			if (&v == p.v1 || &v == p.v2 || &v == p.v3) {  //pronalazak svih incidentnih poligona
				incidentPolygons.push_back(p);
			}
		}
		int size = incidentPolygons.size();
		float a = 0, b = 0, c = 0;
		for (auto &p : incidentPolygons) {   //racunanje normala za vrh kroz sve poligone koji sadrze taj vrh
			a += p.n.a;
			b += p.n.b;
			c += p.n.c;
		}
		v.n.a = a / size;
		v.n.b = b / size;
		v.n.c = c / size;

		float length = sqrt(pow(v.n.a, 2) + pow(v.n.b, 2) + pow(v.n.c, 2)); //normiranje
		v.n.a = v.n.a / length;
		v.n.b = v.n.b / length;
		v.n.c = v.n.c / length;

		cout  << v.n.a << " " << v.n.b << " " << v.n.c << endl;
	}
}

float constantIntensity(Poligon &p) {
	float ka = 0.5, kd = 0.8;
	int Ia = 150, Ii = 200;
	vec3 l, n;
	float center_x = (p.v1->x + p.v2->x + p.v3->x) / 3;
	float center_y = (p.v1->y + p.v2->y + p.v3->y) / 3;
	float center_z = (p.v1->z + p.v2->z + p.v3->z) / 3;
	l =  -vec3(center_x, center_y, center_z) + vec3(light.x, light.y, light.z);
	n = vec3(p.n.a, p.n.b, p.n.c);
	float Id = Ii*kd*dot(l, n);
	if (Id < 0) Id = 0;
	return (Ia*ka + Id);
}

void constantShading() {
	float I;
	for (auto &p : polygons) {
		I = constantIntensity(p);
		glBegin(GL_TRIANGLES);
		if (p.front) {
			glColor3ub(I, 0, 0);
			glVertex3f(p.v1->x, p.v1->y, p.v1->z);
			glVertex3f(p.v2->x, p.v2->y, p.v2->z);
			glVertex3f(p.v3->x, p.v3->y, p.v3->z);
		}
		glEnd();
	}
	glutSwapBuffers();
}

float gouraudIntensity(Vrh &v) {
	float ka = 0.5, kd = 0.8;
	int Ia = 150, Ii = 200;
	vec3 l, n;
	l = vec3(v.x, v.y, v.z) - vec3(light.x, light.y, light.z);
	n = vec3(v.n.a, v.n.b, v.n.c);
	int Id = Ii*kd*dot(l, n);
	if (Id < 0) Id = 0;
	return Ia*ka + Id;
}

void gouraudShading() {
	int I1, I2, I3;
	for (auto &p : polygons) {
		I1 = gouraudIntensity(*(p.v1));
		I2 = gouraudIntensity(*(p.v2));
		I3 = gouraudIntensity(*(p.v3));
		glBegin(GL_TRIANGLES);
		if (p.front) {
			glColor3ub(I1, 0, 0);
			glVertex3f(p.v1->x, p.v1->y, p.v1->z);
			glColor3ub(I2, 0, 0);
			glVertex3f(p.v2->x, p.v2->y, p.v2->z);
			glColor3ub(I3, 0, 0);
			glVertex3f(p.v3->x, p.v3->y, p.v3->z);
		}
		glEnd();
	}
	glutSwapBuffers();
}
