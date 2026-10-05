#include <GL/freeglut.h>
float shade=0.15f;
void display(){glClearColor(shade,shade,shade,1);glClear(GL_COLOR_BUFFER_BIT);glutSwapBuffers();}
void entry(int state){shade=state==GLUT_ENTERED?0.8f:0.2f;glutPostRedisplay();}
int main(int argc,char** argv){glutInit(&argc,argv);glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);glutInitWindowSize(600,400);glutCreateWindow("Q11 - Entry-Driven Background");glutDisplayFunc(display);glutEntryFunc(entry);glutMainLoop();}
