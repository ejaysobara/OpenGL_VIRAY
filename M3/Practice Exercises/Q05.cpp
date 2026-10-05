#include <GL/freeglut.h>
int message = 0;
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (message) {
        glColor3f(message == 1 ? 0 : 1, message == 1 ? 1 : 0, 0);
        glRasterPos2f(-0.45f, 0);
        const char* s = message == 1 ? "Left button text" : "Right button text";
        for (; *s; ++s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *s);
    }
    glutSwapBuffers();
}
void mouse(int button, int state, int, int) {
    if (state != GLUT_DOWN) return;
    if (button == GLUT_LEFT_BUTTON) message = 1;
    if (button == GLUT_RIGHT_BUTTON) message = 2;
    glutPostRedisplay();
}
int main(int argc,char** argv) {
    glutInit(&argc,argv); glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB); glutInitWindowSize(600,400);
    glutCreateWindow("Q05 - Mouse Button Text Color"); glutDisplayFunc(display); glutMouseFunc(mouse); glutMainLoop();
}
