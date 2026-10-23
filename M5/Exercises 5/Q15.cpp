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
 * Q15 - Continuous Bouncing glMapBuffer Animation [Hard]
 * The quad's bottom stays put; the timer raises and lowers its top edge,
 * flipping direction at TOP_MIN and TOP_MAX.
 */

GLuint VBOid;
const float TOP_MIN = -0.2f, TOP_MAX = 0.8f;
float topY = 0.3f;     // current height of the top edge
float dy   = 0.02f;    // current direction and speed

void init() {
    GLfloat quad[] = {
        -0.3f, -0.6f, 0.0f,   // 0 bottom-left
         0.3f, -0.6f, 0.0f,   // 1 bottom-right
         0.3f,  topY, 0.0f,   // 2 top-right  (y = mapped[7])
        -0.3f,  topY, 0.0f    // 3 top-left   (y = mapped[10])
    };

    glGenBuffers(1, &VBOid);
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.3f, 0.8f, 0.9f);

    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glFlush();
}

void bounce(int) {
    topY += dy;
    if (topY >= TOP_MAX || topY <= TOP_MIN) {
        topY = (topY >= TOP_MAX) ? TOP_MAX : TOP_MIN;
        dy = -dy;
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    GLfloat* mapped = (GLfloat*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    if (mapped) {
        mapped[7]  = topY;
        mapped[10] = topY;
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glutPostRedisplay();
    glutTimerFunc(30, bounce, 0);
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
    glutCreateWindow("Q15 - Bouncing glMapBuffer Quad");
#ifndef __APPLE__
    if (glewInit() != GLEW_OK) return 1;
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(monitorKeyboard);
    glutTimerFunc(30, bounce, 0);
    glutMainLoop();
    return 0;
}
