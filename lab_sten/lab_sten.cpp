#include "stdafx.h"
#include "GL/glut.h"
#include <stdio.h>
#include <math.h>

#define LABIRINT_WIDTH 6   // количество клеток лабиринта по ширине
#define LABIRINT_HEIGHT 8   // количество клеток лабиринта по высоте
#define CELL_AREA 45  // площадь клетки в пикселях

#define WALL_NO     0 // 0    нет стенок
#define WALL_BOTTOM 1 // 1 _  нижняя стенка
#define WALL_RIGHT  2 // 2  | правая стенка
#define WALL_BOTH   3 // 3 _| нижняя и правая стенки

int pole[LABIRINT_HEIGHT][LABIRINT_WIDTH] = {
	2, 1, 0, 0, 1, 2,
	2, 1, 3, 3, 3, 2,
	2, 0, 0, 0, 0, 3,
	0, 3, 1, 3, 0, 2,
	3, 0, 0, 0, 0, 3,
	0, 3, 1, 3, 0, 2,
	3, 0, 0, 3, 3, 2,
	1, 1, 1, 1, 1, 3
};

int hero_X = 1;
int hero_Y = 1;


void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, 640.0, 0.0, 480.0, -20.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void renderBitmapString(float x, float y, float z, void *font, char *string)
{
	char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}


void glLine2f(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINE_STRIP);
	glVertex3f(x1, y1, 0.0);
	glVertex3f(x2, y2, 0.0);
	glEnd();
}


void hero()
{
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(CELL_AREA / 2, CELL_AREA / 2, 0);
	glBegin(GL_POLYGON);
	glVertex3f(hero_X - 10, hero_Y - 10, 0.0);
	glVertex3f(hero_X - 10, hero_Y + 10, 0.0);
	glVertex3f(hero_X + 10, hero_Y + 10, 0.0);
	glVertex3f(hero_X + 10, hero_Y - 10, 0.0);
	glEnd();
	glTranslatef(-CELL_AREA / 2, -CELL_AREA / 2, 0);
}


void printWall(int type, int x, int y) {
	switch (type)
	{
	case WALL_BOTTOM:
		glLine2f(x * CELL_AREA, y * CELL_AREA, (x + 1) * CELL_AREA, y * CELL_AREA);
		break;
	case WALL_RIGHT:
		glLine2f((x + 1) * CELL_AREA, y * CELL_AREA, (x + 1) * CELL_AREA, (y + 1) * CELL_AREA);
		break;
/*	case WALL_RIGHT:
		glLine2f((x + 1) * CELL_AREA, y * CELL_AREA, (x + 1) * CELL_AREA, (y + 1) * CELL_AREA);
		break;*/
	case WALL_BOTH:
		glLine2f(x * CELL_AREA, y * CELL_AREA, (x + 1) * CELL_AREA, y * CELL_AREA);
		glLine2f((x + 1) * CELL_AREA, y * CELL_AREA, (x + 1) * CELL_AREA, (y + 1) * CELL_AREA);
		break;
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.5, 1.0, 0);

	glTranslatef(CELL_AREA, CELL_AREA, 0);
	glColor3f(0.0, 1.0, 0.5);
	glRectf(0, 0, LABIRINT_WIDTH*CELL_AREA, LABIRINT_HEIGHT*CELL_AREA);

	glLineWidth(7);   //размер линий

	// стены
	glColor3f(1.0, 0.0, 1.0);
	for (int i = 0; i < LABIRINT_WIDTH; i++)
		for (int j = 0; j < LABIRINT_HEIGHT; j++)
		{
			printWall(pole[LABIRINT_HEIGHT - j - 1][i], i, j);
		}

	hero();

	glColor3f(1.0, 0.0, 1.0);
	glLine2f(0, 0, 0, LABIRINT_HEIGHT*CELL_AREA);
	glLine2f(0, LABIRINT_HEIGHT*CELL_AREA, LABIRINT_WIDTH*CELL_AREA, LABIRINT_HEIGHT*CELL_AREA);

	glTranslatef(-CELL_AREA, -CELL_AREA, 0);
	glFlush();
	glFinish();
}


void keyboard(unsigned char Key, int X, int Y)
{
	if (Key == 27) exit(30);
}


bool isAbleToGo(int x, int  y, int  direction)
{ 
	int type = pole[x][y];
	switch (direction) {
		case GLUT_KEY_RIGHT:
			if (type != 0){
				return false;
			}
	}
	return true;
}


void keyboard_s(int c, int x, int y)
{
	if (c == GLUT_KEY_LEFT) {

		hero_X -= CELL_AREA;
	}

	if (c == GLUT_KEY_RIGHT) {
		hero_X += (isAbleToGo(x, y, GLUT_KEY_RIGHT) ? CELL_AREA : 1);  //кординаты// надо массив
	}

	if (c == GLUT_KEY_DOWN) {
		hero_Y -= CELL_AREA;
	}

	if (c == GLUT_KEY_UP) {
		hero_Y += CELL_AREA;
	}
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(400, 200);

	glDepthFunc(GL_LESS);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("MAZE (тест)");

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutSpecialFunc(keyboard_s);

	glutMainLoop();

	return 0;
}




