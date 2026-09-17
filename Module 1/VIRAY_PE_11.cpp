#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

void drawCircleOutline(float radius, float red, float green, float blue)
{
    glColor3f(red, green, blue);

    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 60; i++)
    {
        float angle = 2.0f * 3.14159265f * i / 60.0f;

        float x = radius * cos(angle);
        float y = radius * sin(angle);

        glVertex2f(x, y);
    }

    glEnd();
}

void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(3.0f);

    drawCircleOutline(0.75f, 1.0f, 0.0f, 0.0f);
    drawCircleOutline(0.50f, 0.0f, 1.0f, 0.0f);
    drawCircleOutline(0.25f, 0.0f, 0.0f, 1.0f);
    

    glFlush();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("PE 11 - Concentric Circles");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}