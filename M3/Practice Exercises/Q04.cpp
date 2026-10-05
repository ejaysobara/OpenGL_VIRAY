#include <GL/freeglut.h>
float bg[3] = {0, 0, 0};
void display() { glClearColor(bg[0], bg[1], bg[2], 1); glClear(GL_COLOR_BUFFER_BIT); glutSwapBuffers(); }
void key(unsigned char k, int, int) {
    if (k == 'r') { bg[0]=1; bg[1]=bg[2]=0; }
    else if (k == 'g') { bg[1]=1; bg[0]=bg[2]=0; }
    else if (k == 'b') { bg[2]=1; bg[0]=bg[1]=0; }
    else return;
    glutPostRedisplay();
}
int main(int argc,char** argv) {
    glutInit(&argc,argv); glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB); glutInitWindowSize(600,400);
    glutCreateWindow("Q04 - Keyboard Background Color"); glutDisplayFunc(display); glutKeyboardFunc(key); glutMainLoop();
}
