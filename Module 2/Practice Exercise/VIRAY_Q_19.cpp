#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClearColor(0.05f, 0.05f, 0.08f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glColor4f(1, 0.1f, 0.1f, 0.5f);
    glVertex2f(-0.7f, -0.45f); glVertex2f(0.2f, -0.45f);
    glVertex2f(0.2f, 0.45f); glVertex2f(-0.7f, 0.45f);
    glColor4f(0.1f, 0.3f, 1, 0.5f);
    glVertex2f(-0.2f, -0.45f); glVertex2f(0.7f, -0.45f);
    glVertex2f(0.7f, 0.45f); glVertex2f(-0.2f, 0.45f);
    glEnd();
    glDisable(GL_BLEND);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q19 - Overlapping Alpha-Blended Quads");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
