#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <cmath>
#include <cstdlib>
using namespace std;

/*
 * Q03 - Static Hexagon With the Correct Usage Hint [Easy]
 */

GLuint VBOid;

void init() {
    // Six corners, counter-clockwise, radius 0.6
    GLfloat hexVertices[6 * 3];
    for (int i = 0; i < 6; i++) {
        float a = 3.14159265f / 3.0f * i;
        hexVertices[i*3 + 0] = 0.6f * cosf(a);
        hexVertices[i*3 + 1] = 0.6f * sinf(a);
        hexVertices[i*3 + 2] = 0.0f;
    }

    glGenBuffers(1, &VBOid);
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    // GL_STATIC_DRAW: the hexagon is uploaded once and never edited, but it is
    // drawn every frame. That "set once, draw many" pattern is exactly what
    // STATIC_DRAW tells the driver, so it can keep the data in fast GPU memory.
    glBufferData(GL_ARRAY_BUFFER, sizeof(hexVertices), hexVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.95f, 0.75f, 0.1f);

    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_POLYGON, 0, 6);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glFlush();
}

void monitorKeyboard(unsigned char key, int, int) {
    if (key == 27) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &VBOid);
        exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Q03 - Static Hexagon");
#ifndef __APPLE__
    if (glewInit() != GLEW_OK) return 1;
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(monitorKeyboard);
    glutMainLoop();
    return 0;
}
