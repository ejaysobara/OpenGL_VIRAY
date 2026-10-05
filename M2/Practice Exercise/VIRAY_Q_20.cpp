#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

constexpr int SEGMENT_COUNT = 72;
constexpr float PI = 3.14159265f;

void rainbow(float t) {
    glColor3f(
        0.5f + 0.5f * std::cos(2 * PI * t),
        0.5f + 0.5f * std::cos(2 * PI * (t - 1.0f / 3)),
        0.5f + 0.5f * std::cos(2 * PI * (t - 2.0f / 3)));
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1, 1, 1); glVertex2f(0, 0);
    for (int i = 0; i <= SEGMENT_COUNT; ++i) {
        float t = static_cast<float>(i) / SEGMENT_COUNT;
        float angle = t * 2 * PI;
        rainbow(t);
        glVertex2f(0.75f * std::cos(angle), 0.75f * std::sin(angle));
    }
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q20 - Procedural Rainbow Fan");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
