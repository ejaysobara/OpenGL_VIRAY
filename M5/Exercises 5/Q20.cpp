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
 * Q20 - Capstone: Animated Ship Over Indexed Ground [Hard]
 *   Ship   : position VBO (dynamic) + color VBO, moved once per second
 *   Ground : position VBO + index VBO, drawn with glDrawElements
 *   ESC    : releases all FOUR buffers before exit(0)
 */

GLuint shipPosVBO, shipColorVBO, groundVBO, groundIndexVBO;
float shipX = 0.0f;    // ship's current x offset
float dx    = 0.1f;    // step per second; flips at the edges

void init() {
    GLfloat shipPos[] = {
         0.25f, 0.0f, 0.0f,   // nose
        -0.15f, 0.15f, 0.0f,
        -0.15f, -0.15f, 0.0f
    };
    GLfloat shipColor[] = {
        1.0f, 0.9f, 0.2f,
        1.0f, 0.3f, 0.2f,
        0.9f, 0.2f, 0.6f
    };
    GLfloat groundVerts[] = {
        -0.9f, -0.9f, 0.0f,   0.9f, -0.9f, 0.0f,
         0.9f, -0.6f, 0.0f,  -0.9f, -0.6f, 0.0f
    };
    GLubyte groundIndices[] = { 0, 1, 2,  0, 2, 3 };

    glGenBuffers(1, &shipPosVBO);
    glBindBuffer(GL_ARRAY_BUFFER, shipPosVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shipPos), shipPos, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &shipColorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, shipColorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shipColor), shipColor, GL_STATIC_DRAW);

    glGenBuffers(1, &groundVBO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVerts), groundVerts, GL_STATIC_DRAW);

    glGenBuffers(1, &groundIndexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundIndexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundIndices), groundIndices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void drawGround() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundIndexVBO);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glColor3f(0.3f, 0.6f, 0.3f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void drawShip() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, shipPosVBO);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, shipColorVBO);
    glColorPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawGround();
    drawShip();
    glFlush();
}

void moveShip(int) {
    // Keep the whole ship (x from -0.15 to +0.25 around shipX) inside the window
    if (shipX + dx > 0.6f || shipX + dx < -0.7f) dx = -dx;
    shipX += dx;

    glBindBuffer(GL_ARRAY_BUFFER, shipPosVBO);
    GLfloat* mapped = (GLfloat*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if (mapped) {
        mapped[0] += dx; mapped[3] += dx; mapped[6] += dx;  // every vertex's x
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glutPostRedisplay();
    glutTimerFunc(1000, moveShip, 0);
}

void monitorKeyboard(unsigned char key, int, int) {
    if (key == 27) {   // ESC: release every GPU buffer before exiting
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &shipPosVBO);
        glDeleteBuffers(1, &shipColorVBO);
        glDeleteBuffers(1, &groundVBO);
        glDeleteBuffers(1, &groundIndexVBO);
        exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Q20 - Animated Ship Over Indexed Ground");
#ifndef __APPLE__
    if (glewInit() != GLEW_OK) return 1;
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(monitorKeyboard);
    glutTimerFunc(1000, moveShip, 0);
    glutMainLoop();
    return 0;
}
