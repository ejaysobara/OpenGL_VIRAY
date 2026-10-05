#define GL_SILENCE_DEPRECATION
#include <GL/freeglut.h>

void text(float x, float y, const char* s) {
    glRasterPos2f(x, y);
    for (; *s; ++s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *s);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    text(-0.35f, 0, "Earl John Viray"); // Replace with your name.
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Q01 - Display Your Name");
    glClearColor(0, 0, 0, 1);
    glutDisplayFunc(display);
    glutMainLoop();
}
