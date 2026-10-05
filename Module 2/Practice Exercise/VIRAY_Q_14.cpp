#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(3.0f);
    glColor3f(1, 0.7f, 0);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
    glBegin(GL_LINES);
    glVertex2f(-0.8f, 0.3f); glVertex2f(0.8f, 0.3f);
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    glColor3f(0.2f, 0.8f, 1);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0xAAAA);
    glBegin(GL_LINES);
    glVertex2f(-0.8f, -0.3f); glVertex2f(0.8f, -0.3f);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Q14 - Two Stipple Patterns");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
