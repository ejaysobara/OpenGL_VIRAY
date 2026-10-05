#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

void display() {
    constexpr float PI = 3.14159265f;
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.5f, 0.1f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, -0.25f);
    for (int i = 0; i <= 7; ++i) {
        float angle = i * PI / 7;
        glVertex2f(0.65f * std::cos(angle), -0.25f + 0.65f * std::sin(angle));
    }
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q10 - Half-Circle Fan");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
