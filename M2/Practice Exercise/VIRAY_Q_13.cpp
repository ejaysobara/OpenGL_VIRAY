#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    glLineWidth(10.0f);
    glBegin(GL_LINES);
    glColor3f(1, 1, 0); glVertex2f(-0.8f, 0);
    glColor3f(0.5f, 0, 0.8f); glVertex2f(0.8f, 0);
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Q13 - Yellow-to-Purple Gradient Line");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
