#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

float handAngle = 0.0f;
int lastUpdateTime = 0;

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

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(3.0f);

    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 60; i++)
    {
        float angle = 2.0f * 3.14159265f * i / 60.0f;
        glVertex2f(0.7f * cos(angle), 0.7f * sin(angle));
    }
    glEnd();

    glBegin(GL_LINES);

    for (int i = 0; i < 12; i++)
    {
        float angle = 2.0f * 3.14159265f * i / 12.0f;

        glVertex2f(0.58f * cos(angle), 0.58f * sin(angle));
        glVertex2f(0.68f * cos(angle), 0.68f * sin(angle));
    }
    glEnd();

    glLineWidth(5.0f);

    glBegin(GL_LINES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.55f * cos(handAngle),
                0.55f * sin(handAngle));
    glEnd();
   
    glFlush();
}
void update()
{
    int currentTime = glutGet(GLUT_ELAPSED_TIME);

    if (currentTime - lastUpdateTime < 16)
        return;

    lastUpdateTime = currentTime;

    handAngle += 0.02f;

    if (handAngle > 2.0f * 3.14159265f)
        handAngle -= 2.0f * 3.14159265f;

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("PE 18 - Rotating Clock Hand");
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glutMainLoop();
    return 0;
}