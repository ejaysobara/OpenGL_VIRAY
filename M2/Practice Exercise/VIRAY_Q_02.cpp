#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(-0.8f, 0.5f); glVertex2f(-0.2f, 0.5f);
    glVertex2f(0.5f, -0.7f); glVertex2f(0.5f, -0.1f);
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q02 - Two Unconnected Lines");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
