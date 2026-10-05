#include <GL/freeglut.h>
float x = 0;
void display() {
    glClear(GL_COLOR_BUFFER_BIT); glColor3f(1,0.7f,0.1f);
    glBegin(GL_QUADS); glVertex2f(x-0.1f,-0.1f); glVertex2f(x+0.1f,-0.1f); glVertex2f(x+0.1f,0.1f); glVertex2f(x-0.1f,0.1f); glEnd(); glutSwapBuffers();
}
void key(unsigned char k,int,int) { if(k=='a') x-=0.05f; if(k=='d') x+=0.05f; if(x < -0.8f)x=-0.8f; if(x>0.8f)x=0.8f; glutPostRedisplay(); }
int main(int argc,char** argv) {
    glutInit(&argc,argv); glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB); glutInitWindowSize(600,400);
    glutCreateWindow("Q08 - Clamped Keyboard Movement"); glClearColor(0,0,0,1); glutDisplayFunc(display); glutKeyboardFunc(key); glutMainLoop();
}
