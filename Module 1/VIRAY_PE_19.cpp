#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

float backgroundRed = 1.0f;
float backgroundGreen = 1.0f;
float backgroundBlue = 1.0f;

void display()
{
    
    glClearColor(backgroundRed, backgroundGreen, backgroundBlue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    

    glFlush();
}
void keyboard(unsigned char key, int x, int y)
{
    if (key == '1')
    {
        backgroundRed = 1.0f;
        backgroundGreen = 1.0f;
        backgroundBlue = 1.0f;
    }
    else if (key == '2')
    {
        backgroundRed = 1.0f;
        backgroundGreen = 0.0f;
        backgroundBlue = 0.0f;
    }
    else if (key == '3')
    {
        backgroundRed = 0.0f;
        backgroundGreen = 1.0f;
        backgroundBlue = 0.0f;
    }
    else if (key == '4')
    {
        backgroundRed = 0.0f;
        backgroundGreen = 0.0f;
        backgroundBlue = 1.0f;
    }
    else if (key == '5')
    {
        backgroundRed = 1.0f;
        backgroundGreen = 1.0f;
        backgroundBlue = 0.0f;
    }
    else if (key == '6')
    {
        backgroundRed = 0.0f;
        backgroundGreen = 1.0f;
        backgroundBlue = 1.0f;
    }
    else if (key == 27)
    {
        exit(0);
    }

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("PE 19 - Keyboard Color Picker");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}