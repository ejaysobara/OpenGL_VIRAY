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

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(8.0f);

    glBegin(GL_LINES);
        glVertex2f(-0.8f, 0.0f);
        glVertex2f(0.4f, 0.0f);
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_TRIANGLES);
        glVertex2f(0.8f, 0.0f);
        glVertex2f(0.3f, 0.35f);
        glVertex2f(0.3f, -0.35f);
    glEnd();

    
    

    glFlush();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("PE 12 - Arrow");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}