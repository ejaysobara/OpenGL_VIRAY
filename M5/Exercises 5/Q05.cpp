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
 * Q05 - Two Separate VBOs Side by Side [Easy]
 */

GLuint leftVBO, rightVBO;

GLuint makeVBO(const GLfloat* verts, GLsizeiptr bytes) {
    GLuint id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, bytes, verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return id;
}

void init() {
    GLfloat leftVerts[]  = { -0.5f, 0.4f, 0.0f,  -0.8f, -0.3f, 0.0f,  -0.2f, -0.3f, 0.0f };
    GLfloat rightVerts[] = {  0.5f, 0.4f, 0.0f,   0.2f, -0.3f, 0.0f,   0.8f, -0.3f, 0.0f };
    leftVBO  = makeVBO(leftVerts,  sizeof(leftVerts));
    rightVBO = makeVBO(rightVerts, sizeof(rightVerts));
}

void drawVBO(GLuint id) {
    glBindBuffer(GL_ARRAY_BUFFER, id); // re-bind right before the draw
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColor3f(1.0f, 0.4f, 0.1f);
    drawVBO(leftVBO);
    glColor3f(0.1f, 0.4f, 1.0f);
    drawVBO(rightVBO);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFlush();
}

void monitorKeyboard(unsigned char key, int, int) {
    if (key == 27) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &leftVBO);
        glDeleteBuffers(1, &rightVBO);
        exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Q05 - Two Separate VBOs");
#ifndef __APPLE__
    if (glewInit() != GLEW_OK) return 1;
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(monitorKeyboard);
    glutMainLoop();
    return 0;
}
