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
 * Q18 - Interleaved + Indexed Hexagon [Hard]
 * One interleaved [x,y,z,r,g,b] buffer plus a separate index buffer {0..5}.
 * A convex hexagon fans cleanly from vertex 0, so GL_TRIANGLE_FAN fills it.
 */

GLuint VBOid, VBOindex;
const GLsizei STRIDE = 6 * sizeof(GLfloat);

void init() {
    const GLfloat rgb[6][3] = {
        {1.0f, 0.2f, 0.2f}, {1.0f, 0.8f, 0.1f}, {0.2f, 0.9f, 0.3f},
        {0.1f, 0.8f, 0.9f}, {0.3f, 0.3f, 1.0f}, {0.9f, 0.3f, 0.9f}
    };
    GLfloat data[6 * 6];
    for (int i = 0; i < 6; i++) {
        float a = 3.14159265f / 3.0f * i;
        GLfloat* p = &data[i * 6];
        p[0] = 0.6f * cosf(a); p[1] = 0.6f * sinf(a); p[2] = 0.0f;
        p[3] = rgb[i][0];      p[4] = rgb[i][1];      p[5] = rgb[i][2];
    }
    GLubyte indices[] = { 0, 1, 2, 3, 4, 5 };

    glGenBuffers(1, &VBOid);
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    glGenBuffers(1, &VBOindex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOindex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOindex);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, STRIDE, (void*)0);
    glColorPointer(3, GL_FLOAT, STRIDE, (void*)(3 * sizeof(GLfloat)));

    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_BYTE, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
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
    glutCreateWindow("Q18 - Interleaved + Indexed Hexagon");
#ifndef __APPLE__
    if (glewInit() != GLEW_OK) return 1;
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(monitorKeyboard);
    glutMainLoop();
    return 0;
}
