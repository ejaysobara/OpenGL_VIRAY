#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 0.0f);
    glLineWidth(4.0f);

    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 10; i++)
    {
        float radius;

        if (i % 2 == 0)
            radius = 0.7f;
        else
            radius = 0.3f;

        float angle = 3.14159265f / 2.0f + i * 3.14159265f / 5.0f;

        float x = radius * cos(angle);
        float y = radius * sin(angle);

        glVertex2f(x, y);
    }

    glEnd();

    glFlush();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("PE 10 - Star");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}