#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.25f, 0.7f, 0.9f);
    glBegin(GL_QUAD_STRIP);
    glVertex2f(-0.8f, -0.6f); glVertex2f(-0.8f, -0.3f);
    glVertex2f(-0.4f, -0.6f); glVertex2f(-0.4f, -0.1f);
    glVertex2f(0, -0.6f); glVertex2f(0, 0.2f);
    glVertex2f(0.4f, -0.6f); glVertex2f(0.4f, 0.5f);
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q12 - Staircase Ribbon");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
