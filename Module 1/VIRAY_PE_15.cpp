#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

float squareX = 0.0f;
float squareY = 0.0f;
float moveAmount = 0.1f;

void drawSquare()
{
    glBegin(GL_QUADS);
        glVertex2f(squareX - 0.2f, squareY - 0.2f);
        glVertex2f(squareX + 0.2f, squareY - 0.2f);
        glVertex2f(squareX + 0.2f, squareY + 0.2f);
        glVertex2f(squareX - 0.2f, squareY + 0.2f);
    glEnd();
}

void display()
{
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 0.0f, 0.0f);
    drawSquare();

    glFlush();
}
void specialKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT)
        squareX -= moveAmount;
    else if (key == GLUT_KEY_RIGHT)
        squareX += moveAmount;
    else if (key == GLUT_KEY_UP)
        squareY += moveAmount;
    else if (key == GLUT_KEY_DOWN)
        squareY -= moveAmount;

    if (squareX > 0.8f) squareX = 0.8f;
    if (squareX < -0.8f) squareX = -0.8f;
    if (squareY > 0.8f) squareY = 0.8f;
    if (squareY < -0.8f) squareY = -0.8f;

    glutPostRedisplay();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("PE 15 - Move Square");
    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutMainLoop();
    return 0;
}