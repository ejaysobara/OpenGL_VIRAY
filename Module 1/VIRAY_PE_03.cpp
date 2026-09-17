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
    glColor3f(0.0f, 0.0f, 1.0f);;
    
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.0f, 0.7f);    // top
        glVertex2f(-0.7f, -0.5f);  // bottom-left
        glVertex2f(0.7f, -0.5f);   // bottom-right
    glEnd();

    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("PE 03 - Triangle Outline");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}