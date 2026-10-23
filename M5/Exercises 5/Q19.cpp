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
 * Q19 - Keyboard-Triggered Color Change via glMapBuffer [Hard]
 * Press 'c' to map the COLOR buffer and write the next scheme into it.
 */

GLuint posVBO, colorVBO;
int scheme = 0;

const int SCHEMES = 3;
const GLfloat SCHEME_RGB[SCHEMES][9] = {
    { 1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f },   // RGB
    { 1.0f, 0.5f, 0.0f,   1.0f, 0.9f, 0.1f,   0.9f, 0.1f, 0.3f },   // warm
    { 0.1f, 0.9f, 0.9f,   0.3f, 0.3f, 1.0f,   0.7f, 0.3f, 0.9f }    // cool
};

void init() {
    GLfloat positions[] = {
         0.0f,  0.7f, 0.0f,
        -0.7f, -0.5f, 0.0f,
         0.7f, -0.5f, 0.0f
    };

    glGenBuffers(1, &posVBO);
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    // colors get rewritten on every 'c' press
    glBufferData(GL_ARRAY_BUFFER, sizeof(SCHEME_RGB[0]), SCHEME_RGB[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    cout << "Press 'c' to cycle colors, ESC to quit.\n";
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glColorPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFlush();
}

void cycleColors() {
    scheme = (scheme + 1) % SCHEMES;

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    GLfloat* mapped = (GLfloat*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    if (mapped) {
        for (int i = 0; i < 9; i++) mapped[i] = SCHEME_RGB[scheme][i];
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glutPostRedisplay();   // show the change right away
}

void monitorKeyboard(unsigned char key, int, int) {
    if (key == 'c' || key == 'C') cycleColors();
    if (key == 27) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &posVBO);
        glDeleteBuffers(1, &colorVBO);
        exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Q19 - Press c to Cycle Colors");
#ifndef __APPLE__
    if (glewInit() != GLEW_OK) return 1;
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(monitorKeyboard);
    glutMainLoop();
    return 0;
}
