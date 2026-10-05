#include <GL/freeglut.h>
#include <cstdio>
int px=0,py=0;
void display(){glClear(GL_COLOR_BUFFER_BIT);char s[80];std::snprintf(s,sizeof s,"Mouse at (%d, %d)",px,py);glColor3f(1,1,1);glRasterPos2f(-0.8f,0);for(char* p=s;*p;++p)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*p);glutSwapBuffers();}
void motion(int x,int y){px=x;py=y;glutPostRedisplay();}
int main(int argc,char** argv){glutInit(&argc,argv);glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);glutInitWindowSize(600,400);glutCreateWindow("Q10 - Passive Motion Pixel Readout");glClearColor(0,0,0,1);glutDisplayFunc(display);glutPassiveMotionFunc(motion);glutMainLoop();}
