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
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_TRIANGLES);
        glVertex2f(-0.75f,  0.6f);
        glVertex2f(-0.95f, -0.6f);
        glVertex2f(-0.2f,  -0.6f);
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);

    glBegin(GL_TRIANGLES);
        glVertex2f(0.45f,  0.6f);
        glVertex2f(0.2f,  -0.6f);
        glVertex2f(0.95f, -0.6f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("PE 07 - Two Triangles");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}