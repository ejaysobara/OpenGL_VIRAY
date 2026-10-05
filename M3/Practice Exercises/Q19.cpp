#include <GL/freeglut.h>
#include <cstdio>
int score=0;float color[3]={0.2f,0.7f,0.3f};
void display(){glClear(GL_COLOR_BUFFER_BIT);glColor3fv(color);glBegin(GL_QUADS);glVertex2f(-.3f,-.3f);glVertex2f(.3f,-.3f);glVertex2f(.3f,.3f);glVertex2f(-.3f,.3f);glEnd();char s[64];std::snprintf(s,sizeof s,"Score: %d",score);glColor3f(1,1,1);glRasterPos2f(-.9f,.8f);for(char* p=s;*p;++p)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*p);glutSwapBuffers();}
void mouse(int b,int state,int,int){if(b==GLUT_LEFT_BUTTON&&state==GLUT_DOWN){++score;int stage=(score/5)%3;color[0]=stage==0?.2f:stage==1?.2f:1;color[1]=stage==0?.7f:stage==1?.4f:.2f;color[2]=stage==0?.3f:stage==1?1:.2f;glutPostRedisplay();}}
int main(int argc,char** argv){glutInit(&argc,argv);glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);glutInitWindowSize(600,400);glutCreateWindow("Q19 - HUD Score with Color Milestones");glClearColor(0,0,0,1);glutDisplayFunc(display);glutMouseFunc(mouse);glutMainLoop();}
