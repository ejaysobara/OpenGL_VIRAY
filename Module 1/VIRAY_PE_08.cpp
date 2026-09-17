#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(4.0f);

    glShadeModel(GL_SMOOTH);
    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.0f, 0.0f); // top: red
    glVertex2f(0.0f, 0.7f);

    glColor3f(0.0f, 1.0f, 0.0f); // right: green
    glVertex2f(0.7f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f); // bottom: blue
    glVertex2f(0.0f, -0.7f);

    glColor3f(1.0f, 1.0f, 0.0f); // left: yellow
    glVertex2f(-0.7f, 0.0f);

    glEnd();

    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("PE 08 - Smooth Shaded Diamond");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}