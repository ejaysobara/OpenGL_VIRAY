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
 * Q14 - Indexed GL_TRIANGLE_FAN Pentagon [Medium]
 * Vertex 0 is the shared center, stored once. Vertices 1-5 are the rim.
 */

GLuint VBOid, VBOindex;

void init() {
    GLfloat verts[6 * 3] = { 0.0f, 0.0f, 0.0f };   // center
    for (int i = 0; i < 5; i++) {
        float a = 3.14159265f / 2.0f + i * 2.0f * 3.14159265f / 5.0f; // start at top
        verts[(i+1)*3 + 0] = 0.6f * cosf(a);
        verts[(i+1)*3 + 1] = 0.6f * sinf(a);
        verts[(i+1)*3 + 2] = 0.0f;
    }
    // center first, then the rim in order, closing back on vertex 1
    GLubyte indices[] = { 0, 1, 2, 3, 4, 5, 1 };

    glGenBuffers(1, &VBOid);
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glGenBuffers(1, &VBOindex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOindex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.6f, 0.0f);

    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOindex);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawElements(GL_TRIANGLE_FAN, 7, GL_UNSIGNED_BYTE, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glFlush();
}

void monitorKeyboard(unsigned char key, int, int) {
    if (key == 27) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &VBOid);
        glDeleteBuffers(1, &VBOindex);
        exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Q14 - Indexed Triangle Fan Pentagon");
#ifndef __APPLE__
    if (glewInit() != GLEW_OK) return 1;
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(monitorKeyboard);
    glutMainLoop();
    return 0;
}
