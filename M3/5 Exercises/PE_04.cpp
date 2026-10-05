#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#endif

#include <iostream>
using namespace std;

float backgroundRed = 0.1f;
float backgroundGreen = 0.1f;
float backgroundBlue = 0.1f;

void drawBitmapString(void* font, const char* text) {
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void display() {
    glClearColor(
        backgroundRed,
        backgroundGreen,
        backgroundBlue,
        1.0f
    );

    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.65f, 0.0f);

    drawBitmapString(
        GLUT_BITMAP_HELVETICA_18,
        "Press R, G, or B"
    );

    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'r' || key == 'R') {
        backgroundRed = 0.8f;
        backgroundGreen = 0.1f;
        backgroundBlue = 0.1f;
    }
    else if (key == 'g' || key == 'G') {
        backgroundRed = 0.1f;
        backgroundGreen = 0.8f;
        backgroundBlue = 0.1f;
    }
    else if (key == 'b' || key == 'B') {
        backgroundRed = 0.1f;
        backgroundGreen = 0.1f;
        backgroundBlue = 0.8f;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Q04 - Keyboard Background Color");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}