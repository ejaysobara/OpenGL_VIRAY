#include <GL/freeglut.h>
#include <cstdio>
int elapsed=0;bool running=false;
void display(){glClear(GL_COLOR_BUFFER_BIT);char s[64];std::snprintf(s,sizeof s,"Elapsed: %d s",elapsed);glColor3f(1,1,1);glRasterPos2f(-0.3f,0);for(char* p=s;*p;++p)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*p);glutSwapBuffers();}
void tick(int){if(running)++elapsed;glutPostRedisplay();glutTimerFunc(1000,tick,0);}
void mouse(int b,int state,int,int){if(state==GLUT_DOWN){if(b==GLUT_LEFT_BUTTON)running=true;if(b==GLUT_RIGHT_BUTTON)running=false;}}
int main(int argc,char** argv){glutInit(&argc,argv);glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);glutInitWindowSize(600,400);glutCreateWindow("Q17 - Mouse-Controlled Stopwatch");glClearColor(0,0,0,1);glutDisplayFunc(display);glutMouseFunc(mouse);glutTimerFunc(1000,tick,0);glutMainLoop();}
