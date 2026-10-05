#include <GL/freeglut.h>
void display() { glClear(GL_COLOR_BUFFER_BIT); glutSwapBuffers(); }
int main(int argc, char** argv) {
    glutInit(&argc, argv); glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 500); glutInitWindowPosition(150, 150);
    glutCreateWindow("My Custom Window"); glutDisplayFunc(display); glutMainLoop();
}
