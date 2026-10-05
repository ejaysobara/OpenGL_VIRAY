#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

void drawBase() {
    glColor3f(0.9f, 0.6f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(-0.18f, -0.7f); glVertex2f(0.18f, -0.7f);
    glVertex2f(0.18f, -0.05f); glVertex2f(-0.18f, -0.05f);
    glEnd();
}

void drawCircle() {
    constexpr int SEGMENTS = 40;
    constexpr float PI = 3.14159265f;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1, 0.9f, 0.5f); glVertex2f(0, 0.18f);
    for (int i = 0; i <= SEGMENTS; ++i) {
        float angle = 2 * PI * i / SEGMENTS;
        glColor3f(1, 0.2f, 0.5f);
        glVertex2f(0.35f * std::cos(angle), 0.18f + 0.35f * std::sin(angle));
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBase();
    drawCircle();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q16 - Fan and Quad Composition");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
