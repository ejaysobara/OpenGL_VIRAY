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
 * Q16 - Three-Shape VBO Scene: Sun, Indexed Ground, Tree [Hard]
 *   Sun    : glDrawElements, triangle fan with one shared center vertex
 *   Ground : glDrawElements, 4 corners + 6 indices
 *   Tree   : glDrawArrays, trunk quad + crown triangle in one VBO
 */

const int SUN_SEG = 16;
GLuint sunVBO, sunIBO, groundVBO, groundIBO, treeVBO;

void initSun() {
    GLfloat v[(SUN_SEG + 1) * 3] = { 0.65f, 0.65f, 0.0f };  // center
    for (int i = 0; i < SUN_SEG; i++) {
        float a = 2.0f * 3.14159265f * i / SUN_SEG;
        v[(i+1)*3 + 0] = 0.65f + 0.15f * cosf(a);
        v[(i+1)*3 + 1] = 0.65f + 0.15f * sinf(a);
        v[(i+1)*3 + 2] = 0.0f;
    }
    GLubyte idx[SUN_SEG + 2];
    for (int i = 0; i <= SUN_SEG; i++) idx[i] = (GLubyte)i;
    idx[SUN_SEG + 1] = 1;   // close the fan

    glGenBuffers(1, &sunVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
    glGenBuffers(1, &sunIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sunIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
}

void initGround() {
    GLfloat v[] = {
        -1.0f, -1.0f, 0.0f,   1.0f, -1.0f, 0.0f,
         1.0f, -0.5f, 0.0f,  -1.0f, -0.5f, 0.0f
    };
    GLubyte idx[] = { 0, 1, 2,  0, 2, 3 };

    glGenBuffers(1, &groundVBO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
    glGenBuffers(1, &groundIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
}

void initTree() {
    GLfloat v[] = {
        // trunk (GL_QUADS, vertices 0-3)
        -0.45f, -0.6f, 0.0f,  -0.35f, -0.6f, 0.0f,
        -0.35f, -0.2f, 0.0f,  -0.45f, -0.2f, 0.0f,
        // crown (GL_TRIANGLES, vertices 4-6)
        -0.65f, -0.2f, 0.0f,  -0.15f, -0.2f, 0.0f,  -0.40f, 0.35f, 0.0f
    };

    glGenBuffers(1, &treeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, treeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
}

void init() {
    initSun();
    initGround();
    initTree();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glClearColor(0.45f, 0.7f, 0.95f, 1.0f);   // sky
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColor3f(0.25f, 0.6f, 0.25f);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundIBO);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

    glColor3f(1.0f, 0.85f, 0.1f);
    glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sunIBO);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawElements(GL_TRIANGLE_FAN, SUN_SEG + 2, GL_UNSIGNED_BYTE, 0);

    glBindBuffer(GL_ARRAY_BUFFER, treeVBO);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glColor3f(0.45f, 0.28f, 0.12f);
    glDrawArrays(GL_QUADS, 0, 4);
    glColor3f(0.1f, 0.45f, 0.15f);
    glDrawArrays(GL_TRIANGLES, 4, 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glFlush();
}

void monitorKeyboard(unsigned char key, int, int) {
    if (key == 27) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        GLuint ids[] = { sunVBO, sunIBO, groundVBO, groundIBO, treeVBO };
        glDeleteBuffers(5, ids);
        exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Q16 - Sun, Indexed Ground, Tree");
#ifndef __APPLE__
    if (glewInit() != GLEW_OK) return 1;
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(monitorKeyboard);
    glutMainLoop();
    return 0;
}
