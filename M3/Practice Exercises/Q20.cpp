#include <GL/freeglut.h>
#include <cstdio>
#include <cmath>
float x=0,y=0,phase=0;int mx=0,my=0;
void toGL(int px,int py,float& ox,float& oy){int w=glutGet(GLUT_WINDOW_WIDTH),h=glutGet(GLUT_WINDOW_HEIGHT);ox=2.0f*px/w-1;oy=1-2.0f*py/h;}
void text(float px,float py,const char* s){glRasterPos2f(px,py);for(;*s;++s)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*s);}
void display(){glClear(GL_COLOR_BUFFER_BIT);float size=.12f+.02f*sinf(phase);glColor3f(.2f,.7f,1);glBegin(GL_QUADS);glVertex2f(x-size,y-size);glVertex2f(x+size,y-size);glVertex2f(x+size,y+size);glVertex2f(x-size,y+size);glEnd();glColor3f(1,1,1);text(x+size+.03f,y,"Marker");char s[64];std::snprintf(s,sizeof s,"Mouse pixel: %d, %d",mx,my);text(-.95f,.88f,s);glutSwapBuffers();}
void click(int b,int state,int px,int py){if(b==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)toGL(px,py,x,y);}
void passive(int px,int py){mx=px;my=py;glutPostRedisplay();}
void idle(){phase+=.03f;glutPostRedisplay();}
int main(int argc,char** argv){glutInit(&argc,argv);glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);glutInitWindowSize(700,500);glutCreateWindow("Q20 - Interactive Text Placer");glClearColor(0,0,0,1);glutDisplayFunc(display);glutMouseFunc(click);glutPassiveMotionFunc(passive);glutIdleFunc(idle);glutMainLoop();}
