#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(4.0f);
    glColor3f(0.0f, 1.0f, 0.0f);;
    
    glBegin(GL_LINES);
        glVertex2f(-0.8f, 0.0f);
        glVertex2f(0.8f, 0.0f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("PE 02 - Green Horizontal Line");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}