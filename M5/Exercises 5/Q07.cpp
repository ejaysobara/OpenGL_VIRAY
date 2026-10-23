#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <cstdlib>
using namespace std;

/*
 * Q07 - VBO Line-Strip Zigzag [Easy]
 * Seven vertices; y flips between +0.3 and -0.3 as x moves right.
 */

const int ZIGZAG_POINTS = 7;
GLuint VBOid;

void init() {
    GLfloat zigzag[ZIGZAG_POINTS * 3];
    for (int i = 0; i < ZIGZAG_POINTS; i++) {
        zigzag[i*3 + 0] = -0.9f + i * (1.8f / (ZIGZAG_POINTS - 1));
        zigzag[i*3 + 1] = (i % 2 == 0) ? -0.3f : 0.3f;
        zigzag[i*3 + 2] = 0.0f;
    }

    glGenBuffers(1, &VBOid);
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(zigzag), zigzag, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 0.0f);
    glLineWidth(3.0f);

    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_LINE_STRIP, 0, ZIGZAG_POINTS);
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
    glutCreateWindow("Q07 - VBO Line-Strip Zigzag");
#ifndef __APPLE__
    if (glewInit() != GLEW_OK) return 1;
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(monitorKeyboard);
    glutMainLoop();
    return 0;
}
