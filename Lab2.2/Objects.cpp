#include<iostream>
#include<stdio.h>
#include<GL\freeglut.h>
#include<glm\glm.hpp>
#include<malloc.h>
#include<fstream>
#include<string>
#include<vector>
using namespace std;
using namespace glm;


typedef struct{
	double x, y, z, h;
} point;

typedef struct{
	int p1, p2, p3;
	double a, b, c, d;
}poly;

point *vrhovi=NULL;
poly *poligoni = NULL;
vector<string>ulaz;
int brVrh = 0;
int brPol = 0;
GLuint window;
GLint w;
GLint h;

vector<string> obradi(string s);
void izracunajPol(void);
void checkPoint(point t);
void load(void);
void myDisplay();
void myReshape(int width, int height);
void printanje();

int main(int argc, char** argv)
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(200, 200);
	glutInit(&argc, argv);

	window = glutCreateWindow("Polygon");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);

	load();
	izracunajPol();
	point check;
	cout << "Unesi koordinate tocke za provjeru:" << endl;
	cin >> check.x >> check.y >> check.z;
	
	//printanje(); 

	checkPoint(check);
	glutMainLoop();
	return 0;
}


vector<string> obradi(string s){
	vector<string> things;

	for (int i = 1; i < s.size(); i++){
		if (s[i]!=' '){
			things.push_back( s.substr(i, s.substr(i).find(" ")));
			string temp = s.substr(i);
			if (s[0] == 'v'){
				i += 3;
			}
		}

	}

	return things;
}


void izracunajPol(void){
	for (int i = 0; i < brPol; i++){
		//y2-y1-->alpha, z3-z2-->beta, z2-z1-->gamma, y3-y1-->delta, x2-x1-->eps, x3-x1-->zeta
		double alpha = vrhovi[poligoni[i].p2 - 1].y - vrhovi[poligoni[i].p1 - 1].y;
		double beta = vrhovi[poligoni[i].p3 - 1].z - vrhovi[poligoni[i].p1 - 1].z;
		double gamma = vrhovi[poligoni[i].p2 - 1].z - vrhovi[poligoni[i].p1 - 1].z;
		double delta = vrhovi[poligoni[i].p3 - 1].y - vrhovi[poligoni[i].p1 - 1].y;
		double eps = vrhovi[poligoni[i].p2 - 1].x - vrhovi[poligoni[i].p1 - 1].x;
		double  zeta = vrhovi[poligoni[i].p3 - 1].x - vrhovi[poligoni[i].p1 - 1].x;

		poligoni[i].a = alpha*beta - gamma*delta;
		poligoni[i].b = -eps*beta + gamma*zeta;
		poligoni[i].c = eps*delta - alpha*zeta;
		poligoni[i].d = -vrhovi[poligoni[i].p1 - 1].x*poligoni[i].a - vrhovi[poligoni[i].p1 - 1].y*poligoni[i].b - vrhovi[poligoni[i].p1 - 1].z*poligoni[i].c;

	}
}


void checkPoint(point t){
	vec3 p = vec3(t.x, t.y, t.z);
	for (int i = 0; i < brPol;i++){
		vec3 pol = vec3(poligoni[i].a, poligoni[i].b, poligoni[i].c);
		if (dot(p, pol) + poligoni[i].d > 0){
			cout << "Tocka je izvan tijela!" << endl;
			return;
		}
	}
	cout << "Tocka je unutar tijela!" << endl;
}


void load(){
	ifstream pol;
	pol.open("C:/Users/Filip/Desktop/kocka.obj", ios::in);
	string c;

	while (getline(pol, c)){

		for (int i = 0; i < c.size(); i++){
			if (c[i] == '#' || c[i] == 'g') break;
			if (c[i] == 'v' || c[i] == 'f'){
				ulaz = obradi(c.substr(i, c.size()));
				if (c[i] == 'v'){
					brVrh++;
					vrhovi = (point*)realloc(vrhovi, sizeof(point)*brVrh);
					vrhovi[brVrh - 1].x = stod(ulaz[0]);
					vrhovi[brVrh - 1].y = stod(ulaz[1]);
					vrhovi[brVrh - 1].z = stod(ulaz[2]);
					vrhovi[brVrh - 1].h = 1;
				}
				else if (c[i] == 'f'){
					brPol++;
					poligoni = (poly*)realloc(poligoni, sizeof(poly)*brPol);
					poligoni[brPol - 1].p1 = stoi(ulaz[0]);
					poligoni[brPol - 1].p2 = stoi(ulaz[1]);
					poligoni[brPol - 1].p3 = stoi(ulaz[2]);
				}

			}
		}
	}
	
	
}

void myReshape(int width, int height){
	w = width; h = height;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w , 0, h );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // boja pozadine
	glClear(GL_COLOR_BUFFER_BIT);		//	brisanje pozadine
	glPointSize(1.0);					//	postavi velicinu tocke za liniju
	glColor3f(0.0f, 0.0f, 0.0f);		//	postavi boju linije
	
}


void myDisplay(void){
	//glScalef(50, 50, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);

	for (int i = 0; i < brPol; i++){
		glBegin(GL_LINES);
		{
			glVertex2d(vrhovi[poligoni[i].p1 - 1].x*100, vrhovi[poligoni[i].p1 - 1].y*100);
			glVertex2d(vrhovi[poligoni[i].p2 - 1].x*100, vrhovi[poligoni[i].p2 - 1].y*100);

			glVertex2d(vrhovi[poligoni[i].p2 - 1].x * 100, vrhovi[poligoni[i].p2 - 1].y * 100);
			glVertex2d(vrhovi[poligoni[i].p3 - 1].x*100, vrhovi[poligoni[i].p3 - 1].y*100);

			glVertex2d(vrhovi[poligoni[i].p3 - 1].x * 100, vrhovi[poligoni[i].p3 - 1].y * 100);
			glVertex2d(vrhovi[poligoni[i].p1 - 1].x * 100, vrhovi[poligoni[i].p1 - 1].y * 100);
			
		}
		glEnd();
	}

	glFlush();

}

void printanje(void){
	for (int i = 0; i < brVrh; i++){
		cout << i + 1 << "." << vrhovi[i].x << vrhovi[i].y << vrhovi[i].z << endl;
	}

	for (int i = 0; i < brPol; i++){
		cout << i + 1 << "." << poligoni[i].p1 << poligoni[i].p2 << poligoni[i].p3 << endl;
	}

}