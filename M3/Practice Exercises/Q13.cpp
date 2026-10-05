#include <GL/freeglut.h>
#include <cstdio>
int count=0;
void display(){glClear(GL_COLOR_BUFFER_BIT);char s[64];std::snprintf(s,sizeof s,"Counter: %d",count);glColor3f(1,1,1);glRasterPos2f(-0.3f,0);for(char* p=s;*p;++p)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*p);glutSwapBuffers();}
void tick(int){++count;glutPostRedisplay();glutTimerFunc(1000,tick,0);}
int main(int argc,char** argv){glutInit(&argc,argv);glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);glutInitWindowSize(600,400);glutCreateWindow("Q13 - Timer-Driven Counter");glClearColor(0,0,0,1);glutDisplayFunc(display);glutTimerFunc(1000,tick,0);glutMainLoop();}
