#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.3f, 0.8f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(-0.6f, -0.5f);
    glVertex2f(0.4f, -0.5f);
    glVertex2f(0.6f, 0.4f);
    glVertex2f(-0.4f, 0.4f);
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q07 - One Quadrilateral");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
