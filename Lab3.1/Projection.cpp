#include<stdio.h>
#include<iostream>
#include<GL\freeglut.h>
#include<glm\glm.hpp>
#include<malloc.h>
#include<fstream>
#include<string>
#include<vector>
#include<math.h>
#include<glm/ext.hpp>
#include<iomanip>


using namespace std;
using namespace glm;


typedef struct{
	double x, y, z, h;
} point;

typedef struct{
	int p1, p2, p3;
	double a, b, c, d;
}poly;

point *vrhovi = NULL;
poly *poligoni = NULL;
vector<string>ulaz;

GLdouble *mP;
GLdouble *mT;
GLdouble *m1, *m2, *m3, *m4, *m5;
double H;
dmat4 t;
dmat4 pro;
int brVrh = 0;
int brPol = 0;
GLuint window;
GLint w;
GLint h;


point ociste;
point glediste;

void load();
vector<string> obradi(string s);
void myReshape(int width, int  height);
void myDisplay(void);
int checkPoint(point t);
void projection(void);
void izracunajPol(void);
void init(void);
void renderScene(void);
GLdouble *calcMat(dmat4 m);
void calcTransf(void);
void printMODEL_VIEW(void);
void sredi(void);
void printanje(void);



int main(int argc, char** argv){


	load();
	izracunajPol();
	projection();
	calcTransf();
	sredi();
	

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(200, 200);
	glutInit(&argc, argv);
	init();
	
	window = glutCreateWindow("Polygon");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);

	glutMainLoop();
	return 0;

}

void init(void){
	glEnable(GL_DEPTH_TEST);
}

void myReshape(int width, int height){
	w = width; h = height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}


void myDisplay(void){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//glMultMatrixd(mT);
	renderScene();
	//printMODEL_VIEW();
	glutSwapBuffers();

}


void load(){
	ifstream pol;
	pol.open("C:/Users/Filip/Documents/Visual Studio 2013/Projects/IRGPrimjeri/kocka.obj", ios::in);
	string c;

	while (getline(pol, c)){
		cout << "najs"<<endl;
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


vector<string> obradi(string s){
	vector<string> things;

	for (int i = 1; i < s.size(); i++){
		if (s[i] != ' '){
			things.push_back(s.substr(i, s.substr(i).find(" ")));
			string temp = s.substr(i);
			if (s[0] == 'v'){
				i += 3;
			}
		}

	}

	return things;
}


int checkPoint(point t){
	vec3 p = vec3(t.x, t.y, t.z);
	for (int i = 0; i < brPol; i++){
		vec3 pol = vec3(poligoni[i].a, poligoni[i].b, poligoni[i].c);
		if (dot(p, pol) + poligoni[i].d > 0){
			return 0;
		}
	}
	return 1;
}


void projection(){
	//Ucitaj dvije tocke: ociste i glediste. Ociste upisuj dok nije izvan tijela.
	while (1){
		cout << "Upisi koordinate ocista:" << endl;
		scanf("%lf %lf %lf", &ociste.x, &ociste.y, &ociste.z);
		ociste.h = 1;
		if (!checkPoint(ociste)) break;
		else cout << "Ociste ne smije biti unutar tijela!" << endl;
	}
	cout << "Upisi koordinate gledista:" << endl;
	scanf("%lf %lf %lf", &glediste.x, &glediste.y, &glediste.z);
	glediste.h = 1;

	
	
	 H = sqrt(pow(ociste.x - glediste.x, 2) + pow(ociste.y - glediste.y, 2) + pow(ociste.z - glediste.z, 2));

	GLdouble P[] = { 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 0, 1 / H,
		0, 0, 0, 0 };
	
	mP = (GLdouble*)malloc(16 * sizeof(GLdouble));
	for (int i = 0; i < 16; i++){
		mP[i] = P[i];
	}

	pro = dmat4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 0, 0,
		0, 0, 1 / H, 0);
	
}

void izracunajPol(void){
	for (int i = 0; i < brPol; i++){
		//y2-y1-->alpha, z3-z2-->beta, z2-z1-->gamma, y3-y1-->delta, x2-x1-->eps, x3-x1-->zeta
		double alpha = vrhovi[poligoni[i].p2 - 1].y - vrhovi[poligoni[i].p1 - 1].y;
		double beta = vrhovi[poligoni[i].p3 - 1].z - vrhovi[poligoni[i].p1 - 1].z;
		double	 gamma = vrhovi[poligoni[i].p2 - 1].z - vrhovi[poligoni[i].p1 - 1].z;
		double delta = vrhovi[poligoni[i].p3 - 1].y - vrhovi[poligoni[i].p1 - 1].y;
		double eps = vrhovi[poligoni[i].p2 - 1].x - vrhovi[poligoni[i].p1 - 1].x;
		double  zeta = vrhovi[poligoni[i].p3 - 1].x - vrhovi[poligoni[i].p1 - 1].x;

		poligoni[i].a = alpha*beta - gamma*delta;
		poligoni[i].b = -eps*beta + gamma*zeta;
		poligoni[i].c = eps*delta - alpha*zeta;
		poligoni[i].d = -vrhovi[poligoni[i].p1 - 1].x*poligoni[i].a - vrhovi[poligoni[i].p1 - 1].y*poligoni[i].b - vrhovi[poligoni[i].p1 - 1].z*poligoni[i].c;

	}
}

void renderScene(void){
	glColor3f(0.0f, 1.0f, 0.0f);

	for (int i = 0; i < brPol; i++){
		glBegin(GL_LINE_LOOP);
		{
			glVertex3f(vrhovi[poligoni[i].p1 - 1].x, vrhovi[poligoni[i].p1 - 1].y, vrhovi[poligoni[i].p1 - 1].z);
			glVertex3f(vrhovi[poligoni[i].p2 - 1].x, vrhovi[poligoni[i].p2 - 1].y, vrhovi[poligoni[i].p2 - 1].z);
			glVertex3f(vrhovi[poligoni[i].p3 - 1].x, vrhovi[poligoni[i].p3 - 1].y, vrhovi[poligoni[i].p3 - 1].z);

		}
		glEnd();
	}
}


void calcTransf(void){
	//matrica t1 pomice ishodiste koor sustava u ociste.
	
	dmat4 t1 = dmat4( 1, 0, 0, -ociste.x,
		0, 1, 0, 0 - ociste.y,
		0, 0, 1, -ociste.z,
		0, 0, 0, 1 );
	
	
	
	//matrica t2 rotira z os sustava scene da se poklopi sa sustavom oka za kut alpha 'A'. Odreduju se argumenti matrice sinA i cosA
	double sinA = (glediste.y - ociste.y) / sqrt(pow((glediste.y - ociste.y),2.0) + pow((glediste.x - ociste.x),2.0));
	double cosA = (glediste.x - ociste.x) / sqrt(pow((glediste.y - ociste.y),2.0) + pow(glediste.x - ociste.x,2.0));

	

	dmat4 t2= dmat4( cosA, sinA, 0, 0,
		-sinA, cosA, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 );

	

	//odredivanje pomocnih koef x2 i z2 nakon prve rotacije. Preko njih racunamo sinB i cosB
	double x2 = sqrt(pow(glediste.x - ociste.x,2.0) + pow(glediste.y - ociste.y,2.0));
	double z2 = glediste.z - ociste.z;

	double sinB = x2 / sqrt(pow(x2, 2) + pow(z2, 2));
	double cosB = z2 / sqrt(pow(x2, 2) + pow(z2, 2));

	//matrica t3 rotira y os sustava scene da bi se poklopio sa sustavom oka za kut beta 'B'
	

	dmat4 t3 = dmat4( cosB, 0, -sinB, 0,
		0, 1, 0, 0,
		sinB, 0, cosB, 0,
		0, 0, 0, 1 );

	
	//roacija oko z osi za 90
	
	dmat4 t4 = dmat4( 0, 1, 0, 0,
		-1, 0, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 );

	
	//promjena predznaka x osi
	
	dmat4 t5= dmat4( -1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 );

	t = t1*t2*t3*t4*t5;
	cout << to_string(t) << endl;

	mT = (GLdouble*)malloc(16 * sizeof(GLdouble));
	mT = calcMat(t);

	cout << endl << endl;

	for (int i = 0; i < 16; i++){
		if (!(i % 4))cout << endl;
		cout << setw(10)<<mT[i];
	}
	cout << endl;
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

void printMODEL_VIEW() {
	GLfloat matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	cout << "CURRENT MODEL_VIEW MATRIX: " << endl;
	for (int i = 0; i < 4; i++) {
		cout << setw(10) << matrix[4 * 0 + i];
		cout << setw(10) << matrix[4 * 1 + i];
		cout << setw(10) << matrix[4 * 2 + i];
		cout << setw(10) << matrix[4 * 3 + i];
		cout << endl;
	}
}


void sredi(void){
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
			vrhovi[i].z = vrhovi[i].z/ vrhovi[i].h;
			vrhovi[i].h = 1;
		}
	}

}
void printanje(void){
	for (int i = 0; i < brVrh; i++){
		cout << i + 1 << "." << vrhovi[i].x << vrhovi[i].y << vrhovi[i].z << endl;
	}

	for (int i = 0; i < brPol; i++){
		cout << i + 1 << "." << poligoni[i].p1 << poligoni[i].p2 << poligoni[i].p3 << endl;
	}

}