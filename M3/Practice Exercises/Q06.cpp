#include <GL/freeglut.h>
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.2f, 0.7f, 1);
    glBegin(GL_TRIANGLES); glVertex2f(0, 0.6f); glVertex2f(-0.5f, -0.3f); glVertex2f(0.5f, -0.3f); glEnd();
    glColor3f(1, 1, 1); glRasterPos2f(-0.23f, -0.55f);
    const char* s = "Filled triangle"; for (; *s; ++s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *s);
    glutSwapBuffers();
}
int main(int argc,char** argv) {
    glutInit(&argc,argv); glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB); glutInitWindowSize(600,400);
    glutCreateWindow("Q06 - Shape with Caption"); glClearColor(0,0,0,1); glutDisplayFunc(display); glutMainLoop();
}
