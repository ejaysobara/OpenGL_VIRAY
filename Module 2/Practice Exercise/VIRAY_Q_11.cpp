#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    glColor3f(1, 0.2f, 0.2f);
    glVertex2f(-0.8f, -0.35f); glVertex2f(-0.2f, -0.35f);
    glVertex2f(-0.2f, 0.35f); glVertex2f(-0.8f, 0.35f);
    glColor3f(0.2f, 0.4f, 1);
    glVertex2f(0.2f, -0.35f); glVertex2f(0.8f, -0.35f);
    glVertex2f(0.8f, 0.35f); glVertex2f(0.2f, 0.35f);
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q11 - Two Quads in One Block");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
