#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>


void display()
{
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.1f, 0.6f, 0.1f);

    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(1.0f, -0.35f);
        glVertex2f(-1.0f, -0.35f);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.6f, 0.55f);

        for (int i = 0; i <= 40; i++)
        {
            float angle = 2.0f * 3.14159265f * i / 40.0f;

            float x = 0.6f + 0.18f * cos(angle);
            float y = 0.55f + 0.18f * sin(angle);

            glVertex2f(x, y);
        }

    glEnd();

    

    glFlush();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("PE 14 - Simple Landscape");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}