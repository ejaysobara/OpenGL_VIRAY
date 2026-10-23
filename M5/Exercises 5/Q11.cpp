#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <iostream>
#include <cstdlib>
using namespace std;

/*
 * Q11 - One-Shot glMapBuffer Vertical Shift [Medium]
 * After upload, the top vertex is moved up 0.2 by editing GPU memory once.
 */

GLuint VBOid;

void init() {
    GLfloat triangleVertices[] = {
         0.0f,  0.5f, 0.0f,   // top vertex: y is mapped[1]
        -0.6f, -0.4f, 0.0f,
         0.6f, -0.4f, 0.0f
    };

    glGenBuffers(1, &VBOid);
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_DYNAMIC_DRAW);

    GLfloat* mapped = (GLfloat*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if (mapped) {
        mapped[1] += 0.2f;               // only the top vertex's y
        glUnmapBuffer(GL_ARRAY_BUFFER);  // must unmap before any draw
    } else {
        cerr << "glMapBuffer failed; drawing the original triangle.\n";
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor4f(.16f, .72f, .08f, 1.0f);

    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
    glutCreateWindow("Q11 - One-Shot glMapBuffer Shift");
#ifndef __APPLE__
    if (glewInit() != GLEW_OK) return 1;
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(monitorKeyboard);
    glutMainLoop();
    return 0;
}
