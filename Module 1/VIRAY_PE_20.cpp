#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

const int STRIPE_COUNT = 6;
const float PI = 3.14159265f;

void display()
{
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    float flagLeft = -0.9f;
    float flagRight = 0.9f;
    float flagBottom = -0.6f;
    float flagTop = 0.6f;
    float stripeHeight = (flagTop - flagBottom) / STRIPE_COUNT;

    for (int stripe = 0; stripe < STRIPE_COUNT; stripe++)
    {
        float y0 = flagBottom + stripe * stripeHeight;
        float y1 = y0 + stripeHeight;

        if (stripe % 2 == 0)
            glColor3f(0.0f, 0.2f, 0.7f);
        else
            glColor3f(1.0f, 0.9f, 1.0f);

        glBegin(GL_QUADS);
            glVertex2f(flagLeft, y0);
            glVertex2f(flagRight, y0);
            glVertex2f(flagRight, y1);
            glVertex2f(flagLeft, y1);
        glEnd();
    }

    glColor3f(1.0f, 0.8f, 0.0f);

    glBegin(GL_POLYGON);

    for (int i = 0; i < 10; i++)
    {
        float radius;

        if (i % 2 == 0)
            radius = 0.22f;
        else
            radius = 0.09f;

        float angle = PI / 2.0f + i * PI / 5.0f;

        float x = -0.55f + radius * cos(angle);
        float y = 0.25f + radius * sin(angle);

        glVertex2f(x, y);
    }

    glEnd();
    

    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("PE 20 - Striped Flag with Star");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}