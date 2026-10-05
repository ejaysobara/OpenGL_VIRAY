#include <GL/freeglut.h>
#include <cmath>
bool inside=false;float x=-0.7f,speed=0.01f;
void display(){glClear(GL_COLOR_BUFFER_BIT);glColor3f(1,0.6f,0.2f);glBegin(GL_TRIANGLE_FAN);glVertex2f(x,0);for(int i=0;i<=32;++i){float a=6.2831853f*i/32;glVertex2f(x+0.15f*cosf(a),0.15f*sinf(a));}glEnd();glutSwapBuffers();}
void entry(int s){inside=s==GLUT_ENTERED;}
void idle(){if(inside){x+=speed;if(x>0.8f||x< -0.8f)speed=-speed;glutPostRedisplay();}}
int main(int argc,char** argv){glutInit(&argc,argv);glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);glutInitWindowSize(600,400);glutCreateWindow("Q18 - Entry + Idle Freeze Combo");glClearColor(0,0,0,1);glutDisplayFunc(display);glutEntryFunc(entry);glutIdleFunc(idle);glutMainLoop();}
