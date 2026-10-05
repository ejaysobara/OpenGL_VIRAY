#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    glBegin(GL_QUADS);
    glColor3f(1, 0, 0); glVertex2f(-0.6f, -0.6f);
    glColor3f(0, 1, 0); glVertex2f(0.6f, -0.6f);
    glColor3f(0, 0, 1); glVertex2f(0.6f, 0.6f);
    glColor3f(1, 1, 0); glVertex2f(-0.6f, 0.6f);
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q15 - Smoothly Shaded Square");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
