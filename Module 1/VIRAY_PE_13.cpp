#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

void drawRect(float x0, float y0, float x1, float y1)
{
    glBegin(GL_QUADS);
        glVertex2f(x0, y0);
        glVertex2f(x1, y0);
        glVertex2f(x1, y1);
        glVertex2f(x0, y1);
    glEnd();
}

void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.4f, 1.0f);

    drawRect(-0.7f, 0.5f, 0.7f, 0.75f);    // top bar
    drawRect(-0.7f, 0.0f, 0.45f, 0.25f);    // middle bar
    drawRect(-0.7f, -0.7f, -0.45f, 0.75f);  // vertical bar
    

    glFlush();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("PE 13 - Letter F");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}