#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float startX = -0.8f;
    float startY = -0.8f;
    float cellSize = 0.4f;

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            float x0 = startX + col * cellSize;
            float y0 = startY + row * cellSize;

            if ((row + col) % 2 == 0)
                glColor3f(1.0f, 1.0f, 1.0f);
            else
                glColor3f(0.0f, 0.0f, 0.0f);

            glBegin(GL_QUADS);

                glVertex2f(x0, y0);
                glVertex2f(x0 + cellSize, y0);
                glVertex2f(x0 + cellSize, y0 + cellSize);
                glVertex2f(x0, y0 + cellSize);

            glEnd();
        }
    }

    glFlush();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("PE 09 - Checkerboard");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}