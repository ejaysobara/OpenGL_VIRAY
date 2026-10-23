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
 * Q04 - Full Cleanup Checklist [Easy]
 * The vertex array and the buffer stay enabled/bound while the program runs
 * (like Example 15), so ESC has real state to undo.
 */

GLuint VBOid;

void init() {
    GLfloat triangleVertices[] = {
         0.0f,  0.6f, 0.0f,
        -0.6f, -0.4f, 0.0f,
         0.6f, -0.4f, 0.0f
    };

    glGenBuffers(1, &VBOid);
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.9f, 0.3f, 0.5f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glFlush();
}

void monitorKeyboard(unsigned char key, int, int) {
    if (key == 27) {
        // Undoes glEnableClientState: stop reading positions from an array
        glDisableClientState(GL_VERTEX_ARRAY);

        // Undoes glBindBuffer(GL_ARRAY_BUFFER, VBOid): back to normal pointers
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Undoes glGenBuffers/glBufferData: frees the GPU memory
        glDeleteBuffers(1, &VBOid);

        exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Q04 - Full Cleanup Checklist (ESC to exit)");
#ifndef __APPLE__
    if (glewInit() != GLEW_OK) return 1;
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(monitorKeyboard);
    glutMainLoop();
    return 0;
}
