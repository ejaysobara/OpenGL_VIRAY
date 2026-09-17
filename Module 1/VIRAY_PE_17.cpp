#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

int activeLight = 0;

void drawCircle(float centerX, float centerY,
                float radius, float red, float green, float blue)
{
    glColor3f(red, green, blue);

    glBegin(GL_TRIANGLE_FAN);

        glVertex2f(centerX, centerY);

        for (int i = 0; i <= 40; i++)
        {
            float angle = 2.0f * 3.14159265f * i / 40.0f;
            glVertex2f(centerX + radius * cos(angle),
                       centerY + radius * sin(angle));
        }

    glEnd();
}

void display()
{
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.15f, 0.15f, 0.15f);

    glBegin(GL_QUADS);
        glVertex2f(-0.3f, -0.8f);
        glVertex2f(0.3f, -0.8f);
        glVertex2f(0.3f, 0.8f);
        glVertex2f(-0.3f, 0.8f);
    glEnd();
   
    if (activeLight == 0)
    drawCircle(0.0f, 0.45f, 0.18f, 1.0f, 0.0f, 0.0f);
    else
        drawCircle(0.0f, 0.45f, 0.18f, 0.3f, 0.0f, 0.0f);

    if (activeLight == 1)
        drawCircle(0.0f, 0.0f, 0.18f, 1.0f, 1.0f, 0.0f);
    else
        drawCircle(0.0f, 0.0f, 0.18f, 0.3f, 0.3f, 0.0f);

    if (activeLight == 2)
        drawCircle(0.0f, -0.45f, 0.18f, 0.0f, 1.0f, 0.0f);
    else
        drawCircle(0.0f, -0.45f, 0.18f, 0.0f, 0.3f, 0.0f);
    

    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 'n' || key == 'N')
    {
        activeLight = (activeLight + 1) % 3;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("PE 17 - Traffic Light");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}