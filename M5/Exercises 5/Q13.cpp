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
 * Q13 - ESC Cleanup Releasing Two VBOs [Medium]
 * A simple scene: a square block with a triangle flag on top.
 */

GLuint blockVBO, flagVBO;

void init() {
    GLfloat block[] = {
        -0.4f, -0.6f, 0.0f,   0.4f, -0.6f, 0.0f,
         0.4f,  0.1f, 0.0f,  -0.4f,  0.1f, 0.0f
    };
    GLfloat flag[] = {
        -0.4f,  0.1f, 0.0f,   0.4f,  0.1f, 0.0f,   0.0f, 0.6f, 0.0f
    };

    glGenBuffers(1, &blockVBO);
    glBindBuffer(GL_ARRAY_BUFFER, blockVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(block), block, GL_STATIC_DRAW);

    glGenBuffers(1, &flagVBO);
    glBindBuffer(GL_ARRAY_BUFFER, flagVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(flag), flag, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColor3f(0.85f, 0.7f, 0.5f);
    glBindBuffer(GL_ARRAY_BUFFER, blockVBO);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_QUADS, 0, 4);

    glColor3f(0.7f, 0.2f, 0.2f);
    glBindBuffer(GL_ARRAY_BUFFER, flagVBO);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFlush();
}

void monitorKeyboard(unsigned char key, int, int) {
    if (key == 27) { // ESC: release BOTH buffers, then exit
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        GLuint ids[] = { blockVBO, flagVBO };
        glDeleteBuffers(2, ids);
        exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Q13 - ESC Cleanup, Two VBOs");
#ifndef __APPLE__
    if (glewInit() != GLEW_OK) return 1;
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(monitorKeyboard);
    glutMainLoop();
    return 0;
}
