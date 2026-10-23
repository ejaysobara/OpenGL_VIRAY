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
 * Q08 - Indexed Quad: Position VBO + Separate Index VBO [Medium]
 * 4 unique corners on the GPU, 6 indices tell OpenGL how to make 2 triangles.
 */

GLuint VBOid, VBOindex;

void init() {
    GLfloat quadVertices[] = {
        -0.5f, -0.5f, 0.0f,  // 0
         0.5f, -0.5f, 0.0f,  // 1
         0.5f,  0.5f, 0.0f,  // 2
        -0.5f,  0.5f, 0.0f   // 3
    };
    GLubyte indices[] = { 0, 1, 2,  0, 2, 3 };

    glGenBuffers(1, &VBOid);
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &VBOindex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOindex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.9f, 0.5f, 0.1f);

    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOindex);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    // GLubyte indices -> GL_UNSIGNED_BYTE; last argument is a byte OFFSET
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
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
    glutCreateWindow("Q08 - Indexed Quad");
#ifndef __APPLE__
    if (glewInit() != GLEW_OK) return 1;
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(monitorKeyboard);
    glutMainLoop();
    return 0;
}
