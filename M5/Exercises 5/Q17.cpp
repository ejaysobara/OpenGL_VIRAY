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
 * Q17 - Procedural 30-Dash Ring in One VBO [Hard]
 * Each dash covers 60% of its 12-degree slot, leaving a gap before the next.
 */

const int DASHES = 30;
const float RADIUS = 0.6f;
GLuint VBOid;

void init() {
    GLfloat verts[DASHES * 2 * 3];   // 30 dashes x 2 endpoints x (x,y,z)
    float slot = 2.0f * 3.14159265f / DASHES;   // 360/30 degrees, in radians
    for (int i = 0; i < DASHES; i++) {
        float a1 = i * slot;
        float a2 = a1 + slot * 0.6f;
        int b = i * 6;
        verts[b + 0] = RADIUS * cosf(a1); verts[b + 1] = RADIUS * sinf(a1); verts[b + 2] = 0.0f;
        verts[b + 3] = RADIUS * cosf(a2); verts[b + 4] = RADIUS * sinf(a2); verts[b + 5] = 0.0f;
    }

    glGenBuffers(1, &VBOid);
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.2f, 0.8f, 0.9f);
    glLineWidth(4.0f);

    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_LINES, 0, DASHES * 2);   // one call for all 30 dashes
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
    glutCreateWindow("Q17 - 30-Dash Ring");
#ifndef __APPLE__
    if (glewInit() != GLEW_OK) return 1;
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(monitorKeyboard);
    glutMainLoop();
    return 0;
}
