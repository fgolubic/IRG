#include<iostream>
#include<GL\freeglut.h>
#include<stdio.h>
#include<malloc.h>
#include<glm\glm.hpp>
using namespace glm;

GLuint window;
GLuint w, h;



typedef struct {
	int x;
	int y;
} iTocka2D;

typedef struct {
	int a;
	int b;
	int c;
} iBrid2D;

typedef struct {
	iTocka2D Vrh;
	iBrid2D Brid;
	int lijevi;
} iPolyElem;

iPolyElem *vrhovi=NULL;
int brVrhova=0;
int checkFlag = 0;

iPolyElem *checkPoly = NULL;
int checkBrVrhova = 0;

void myDisplay();
void myReshape(int width, int height);
void myMouse(int button, int state, int x, int y);
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
void drawLine(int x1, int y1, int x2, int y2);
void CrtajPoligonKonv(iPolyElem * polel, int n);
void PopuniPoligonKonv(iPolyElem * polel, int n);
void RacunajKoefPoligonKonv(iPolyElem * polel, int n);
void copy(iPolyElem *p1, iPolyElem *p2, int n);
void checkPoint(iTocka2D point, iPolyElem *pol, int n);


int main(int argc, char** argv){

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(200, 200);
	glutInit(&argc, argv);

	window = glutCreateWindow("Polygon");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);

	glutMainLoop();
	return 0;
}


void myReshape(int width, int height){
	w = width; h = height;
	glViewport(0,0,w,h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w - 1, 0, h - 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // boja pozadine
	glClear(GL_COLOR_BUFFER_BIT);		//	brisanje pozadine
	glPointSize(1.0);					//	postavi velicinu tocke za liniju
	glColor3f(0.0f, 0.0f, 0.0f);		//	postavi boju linije
	glRecti(w - 10, h - 10, w, h);
}


void myDisplay(void){
	/*glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glutSwapBuffers();
	*/
	glFlush();
	
	}


void myKeyboard(unsigned char theKey, int mouseX, int mouseY){
	switch (theKey)
	{
	case 'r':
		glColor3f(1, 0, 0);
		break;

	case 'g':
		glColor3f(0, 1, 0);
		break;

	case 'b':
		glColor3f(0, 0, 1);
		break;

	case 'k':
		glColor3f(0, 0, 0);
		break;
	case 'c':
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glFlush();

		if(checkPoly!=NULL) free(checkPoly);
		checkBrVrhova = 0;
		break;
	case 'p':
		checkFlag = checkFlag ^ 1;
		break;

	}
	glRecti(w - 10, h - 10, w, h); // crta mali kvadrat u boji
	glFlush();



}
void myMouse(int button, int state, int x, int y){
	
	if (!checkFlag){
		//Lijeva tipka - uèitavanje koordinata vrhova
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			brVrhova++;
			vrhovi = (iPolyElem*)realloc(vrhovi, brVrhova*sizeof(iPolyElem));
			vrhovi[brVrhova - 1].Vrh.x = x;
			vrhovi[brVrhova - 1].Vrh.y = h - y;
			printf("Koordinate tocke %d: %d %d \n", brVrhova, x, y);

		}

		//	Desna tipka - popunjuje. 
		else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		{
			CrtajPoligonKonv(vrhovi, brVrhova);
			RacunajKoefPoligonKonv(vrhovi, brVrhova);
			PopuniPoligonKonv(vrhovi, brVrhova);

			checkBrVrhova = brVrhova;
			if(checkPoly!=NULL) free(checkPoly);
			checkPoly = (iPolyElem*)malloc(brVrhova*sizeof(iPolyElem));
			copy(vrhovi, checkPoly, brVrhova);
			vrhovi = NULL;
			brVrhova = 0;

		}
		glFlush();
	}
	else{
		if ((button == GLUT_RIGHT_BUTTON || button == GLUT_LEFT_BUTTON) && state == GLUT_DOWN){
			printf("Koordinate tocke za provjeru: %d %d \n",  x, y);
			iTocka2D t;
			t.x = x;
			t.y = h-y;
			checkPoint(t, checkPoly, checkBrVrhova);
		}
	}

	
}
void CrtajPoligonKonv(iPolyElem * polel, int n) {
	int i, i0;
	i0 = n - 1;
	for (i = 0; i < n; i++) {
		drawLine(polel[i0].Vrh.x, polel[i0].Vrh.y, polel[i].Vrh.x, polel[i].Vrh.y);
		i0 = i;
	}
}


void drawLine(GLint x1, GLint y1, GLint x2, GLint y2){
	glBegin(GL_LINES);
	{
		glVertex2i(x1,y1);
		glVertex2i(x2,y2);
	}
	glEnd();
}

void RacunajKoefPoligonKonv(iPolyElem * polel, int n) {
	int i, i0;
	i0 = n-1;
	for (i = 0; i < n; i++) {
		polel[i0].Brid.a = polel[i0].Vrh.y-polel[i].Vrh.y;
		polel[i0].Brid.b = -(polel[i0].Vrh.x-polel[i].Vrh.x);
		polel[i0].Brid.c = polel[i0].Vrh.x* polel[i].Vrh.y-polel[i0].Vrh.y* polel[i].Vrh.x;
		polel[i0].lijevi = polel[i0].Vrh.y < polel[i].Vrh.y;
		i0 = i;
	}
}


void PopuniPoligonKonv(iPolyElem * polel, int n) {
	int i, i0, y;
	int xmin, xmax, ymin, ymax;
	double L, D, x;
	/* Trazenje minimalnih i maksimalnih koordi nata */
	xmin = xmax = polel[0].Vrh.x;
	ymin = ymax = polel[0].Vrh.y;
	for (i = 1; i < n; i++) {
		if(xmin > polel[i].Vrh.x) xmin = polel[i].Vrh.x;
		if(xmax < polel[i].Vrh.x) xmax = polel[i].Vrh.x;
		if(ymin > polel[i].Vrh.y) ymin = polel[i].Vrh.y;
		if(ymax < polel[i].Vrh.y) ymax = polel[i].Vrh.y;
	}
	/* Bojanje poligona : za svaki y izmedu ymin i ymax radi . ..*/
	for (y = ymin; y <= ymax; y++) {
		/* Pronadi najvece lijevo i najmanje desno sjeciste... */
		L = xmin; D = xmax;
		i0 = n - 1;
		/* i 0 je pocetak brida, i je kraj bri da */
		for (i = 0; i < n; i0 = i++) {
			/* ako je brid vodoravan */
			if (polel[i0].Brid.a == 0) {
				if (polel[i0].Vrh.y == y) {
					if (polel[i0].Vrh.x < polel[i].Vrh.x) {
						L = polel[i0].Vrh.x;
						D = polel[i].Vrh.x;
					}
					else{
						L = polel[i].Vrh.x;
						D = polel[i0].Vrh.x;
					}
					break;
				}
			}
			else{ /* inace je regularan brid, nadi sjeciste */
				x = (-polel[i0].Brid.b*y - polel[i0].Brid.c) / (double)polel[i0].Brid.a;
				if (polel[i0].lijevi) {
					if (L < x) L = x;
				}
				else{
					if (D > x) D = x;
				}
			}
		}
		if (L < D){
			drawLine((int)(L + 0.5), y, (int)(D + 0.5), y);
		}
	}
}


void checkPoint(iTocka2D point, iPolyElem *pol, int n){
	for (int i = 0; i < n; i++){
		if ((point.x*pol[i].Brid.a + point.y*pol[i].Brid.b + pol[i].Brid.c)>0){
			printf("Toèka %d , %d je izvan poligona.\n", point.x, point.y);
			return;
		}
	}

	printf("Toèka %d , %d je unutar poligona.\n", point.x, point.y);
}


void copy(iPolyElem *p1, iPolyElem *p2, int n)	{
	
	for (int i = 0; i < n; i++){
		iPolyElem a = p1[i];
		p2[i] = a;
	}
}