#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#endif

#include <iostream>
using namespace std;

void drawBitmapString(void* font, const char* text) {
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);

    glRasterPos2f(-0.45f, 0.5f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_10, "Earl John B. Viray");

    glRasterPos2f(-0.45f, 0.1f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_18, "Earl John B. Viray");

    glRasterPos2f(-0.45f, -0.4f);
    drawBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, "Earl John B. Viray");

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(700, 500);
    glutCreateWindow("Q02 - Font Size Comparison");

    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}