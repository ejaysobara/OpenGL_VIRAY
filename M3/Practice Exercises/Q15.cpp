#include <GL/freeglut.h>
#include <cstdio>
int left=30;bool done=false;
void display(){glClear(GL_COLOR_BUFFER_BIT);char s[64];std::snprintf(s,sizeof s,done?"Time's up!":"Time left: %d",left);glColor3f(1,1,1);glRasterPos2f(-0.35f,0);for(char* p=s;*p;++p)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*p);glutSwapBuffers();}
void tick(int){if(left>0)--left;done=left==0;glutPostRedisplay();if(!done)glutTimerFunc(1000,tick,0);}
int main(int argc,char** argv){glutInit(&argc,argv);glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);glutInitWindowSize(600,400);glutCreateWindow("Q15 - 30-Second Countdown");glClearColor(0,0,0,1);glutDisplayFunc(display);glutTimerFunc(1000,tick,0);glutMainLoop();}
