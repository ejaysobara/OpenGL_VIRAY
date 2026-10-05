#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(9.0f);
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0); glVertex2f(0, 0);
    glColor3f(0, 1, 0); glVertex2f(0, 0.5f);
    glColor3f(0, 0, 1); glVertex2f(0, -0.5f);
    glColor3f(1, 1, 0); glVertex2f(-0.5f, 0);
    glColor3f(1, 0, 1); glVertex2f(0.5f, 0);
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q01 - Plus-Shaped Colored Points");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
