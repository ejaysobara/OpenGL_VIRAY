#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.2f, 0.7f, 1.0f);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(-0.65f, -0.35f);
    glVertex2f(-0.65f, 0.35f);
    glVertex2f(0.65f, -0.35f);
    glVertex2f(0.65f, 0.35f);
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q09 - Rectangle from Triangle Strip");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
