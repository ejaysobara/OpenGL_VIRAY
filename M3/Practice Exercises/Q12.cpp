#include <GL/freeglut.h>
float x=-0.8f,speed=0.01f;
void display(){glClear(GL_COLOR_BUFFER_BIT);glColor3f(1,1,1);glRasterPos2f(x,0);const char* s="Bouncing label";for(;*s;++s)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*s);glutSwapBuffers();}
void idle(){x+=speed;if(x>0.35f||x< -0.95f)speed=-speed;glutPostRedisplay();}
int main(int argc,char** argv){glutInit(&argc,argv);glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);glutInitWindowSize(600,400);glutCreateWindow("Q12 - Idle-Driven Bouncing Label");glClearColor(0,0,0,1);glutDisplayFunc(display);glutIdleFunc(idle);glutMainLoop();}
