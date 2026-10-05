#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glShadeModel(GL_FLAT); // Each quad takes the color of its last vertex.
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= 5; ++i) {
        if (i % 2 == 0) glColor3f(1, 0.3f, 0.1f);
        else glColor3f(0.1f, 0.7f, 1);
        float x = -0.9f + i * 0.36f;
        glVertex2f(x, -0.35f);
        glVertex2f(x, 0.35f);
    }
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q18 - Alternating-Color Ribbon");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
