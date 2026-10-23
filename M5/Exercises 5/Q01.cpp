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
 * Q01 - VBO X-Pattern Points [Easy]
 * Immediate-mode version this replaces:
 *   glBegin(GL_POINTS);
 *     glVertex2f(-0.5f,  0.5f); glVertex2f(0.5f,  0.5f);
 *     glVertex2f( 0.0f,  0.0f);
 *     glVertex2f(-0.5f, -0.5f); glVertex2f(0.5f, -0.5f);
 *   glEnd();
 */

GLuint VBOid;

void init() {
    // 1. vertex data in an ordinary array
    GLfloat pointVertices[] = {
        -0.5f,  0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
         0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

    // 2. generate, bind, upload
    glGenBuffers(1, &VBOid);
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointVertices), pointVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(15.0f);

    // 3. enable the vertex array, 4. point with an OFFSET (0) and draw
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_POINTS, 0, 5);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glFlush();
}

// 5. clean up before exit
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
    glutCreateWindow("Q01 - VBO X-Pattern Points");
#ifndef __APPLE__
    if (glewInit() != GLEW_OK) return 1;
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(monitorKeyboard);
    glutMainLoop();
    return 0;
}
