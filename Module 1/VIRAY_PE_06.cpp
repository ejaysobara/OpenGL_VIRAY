#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display()
{
    glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(4.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    
    glBegin(GL_LINE_LOOP);
        glVertex2f( 0.0f,  0.7f);  // top
        glVertex2f( 0.6f,  0.35f); // upper-right
        glVertex2f( 0.6f, -0.35f); // lower-right
        glVertex2f( 0.0f, -0.7f);  // bottom
        glVertex2f(-0.6f, -0.35f); // lower-left
        glVertex2f(-0.6f,  0.35f); // upper-left
    glEnd();

    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("PE 06 - Hexagon Outline");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}