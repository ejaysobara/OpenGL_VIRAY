#include <GL/freeglut.h>
float y=0;
void display(){glClear(GL_COLOR_BUFFER_BIT);glColor3f(0.3f,0.8f,1);glBegin(GL_QUADS);glVertex2f(-0.15f,y-0.15f);glVertex2f(0.15f,y-0.15f);glVertex2f(0.15f,y+0.15f);glVertex2f(-0.15f,y+0.15f);glEnd();glutSwapBuffers();}
void key(unsigned char k,int,int){if(k=='w')y+=0.05f;if(k=='s')y-=0.05f;if(y>0.85f)y=0.85f;if(y< -0.85f)y=-0.85f;glutPostRedisplay();}
void mouse(int b,int state,int,int){if(b==GLUT_LEFT_BUTTON&&state==GLUT_DOWN){y=0;glutPostRedisplay();}}
int main(int argc,char** argv){glutInit(&argc,argv);glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);glutInitWindowSize(600,400);glutCreateWindow("Q14 - Keyboard + Mouse Combo");glClearColor(0,0,0,1);glutDisplayFunc(display);glutKeyboardFunc(key);glutMouseFunc(mouse);glutMainLoop();}
