#define GL_SILENCE_DEPRECATION
#include <GL/freeglut.h>

void text(float y, void* font, const char* s) {
    glRasterPos2f(-0.5f, y);
    for (; *s; ++s) glutBitmapCharacter(font, *s);
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    text(0.5f, GLUT_BITMAP_HELVETICA_10, "Compare Fonts");
    text(0.0f, GLUT_BITMAP_HELVETICA_18, "Compare Fonts");
    text(-0.5f, GLUT_BITMAP_TIMES_ROMAN_24, "Compare Fonts");
    glutSwapBuffers();
}
int main(int argc, char** argv) {
    glutInit(&argc, argv); glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 400); glutCreateWindow("Q02 - Font Size Comparison");
    glClearColor(0, 0, 0, 1); glutDisplayFunc(display); glutMainLoop();
}
