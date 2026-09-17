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
    glColor3f(1.0f, 0.5f, 0.0f);
    
    glBegin(GL_POLYGON);
        glVertex2f(-0.8f, -0.4f); // bottom-left
        glVertex2f(-0.8f,  0.4f); // top-left
        glVertex2f( 0.8f,  0.4f); // top-right
        glVertex2f( 0.8f, -0.4f); // bottom-right
    glEnd();

    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("PE 04 - Orange Rectangle");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}