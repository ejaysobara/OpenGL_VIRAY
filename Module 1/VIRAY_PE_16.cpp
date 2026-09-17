#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

float ballX = 0.0f;
float ballY = 0.0f;
float ballDX = 0.005f;
float ballDY = 0.004f;

int lastUpdateTime = 0;

void drawBall()
{
    glBegin(GL_TRIANGLE_FAN);

        glVertex2f(ballX, ballY);

        for (int i = 0; i <= 40; i++)
        {
            float angle = 2.0f * 3.14159265f * i / 40.0f;
            float x = ballX + 0.12f * cos(angle);
            float y = ballY + 0.12f * sin(angle);

            glVertex2f(x, y);
        }

    glEnd();
}

void display()
{
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 0.0f, 0.0f);
    drawBall();
    

    glFlush();
}

void update()
{
    int currentTime = glutGet(GLUT_ELAPSED_TIME);

    if (currentTime - lastUpdateTime < 16)
        return;

    lastUpdateTime = currentTime;

    ballX += ballDX;
    ballY += ballDY;

    if (ballX > 0.88f || ballX < -0.88f)
        ballDX = -ballDX;

    if (ballY > 0.88f || ballY < -0.88f)
        ballDY = -ballDY;

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("PE 16 - Bouncing Ball");
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glutMainLoop();
    return 0;
}