#include <GL/freeglut.h>
float x=0,y=0;bool dragging=false;
void display(){glClear(GL_COLOR_BUFFER_BIT);glColor3f(0.2f,0.8f,1);glBegin(GL_QUADS);glVertex2f(x-.12f,y-.12f);glVertex2f(x+.12f,y-.12f);glVertex2f(x+.12f,y+.12f);glVertex2f(x-.12f,y+.12f);glEnd();glutSwapBuffers();}
void pos(int px,int py){x=2.0f*px/glutGet(GLUT_WINDOW_WIDTH)-1;y=1-2.0f*py/glutGet(GLUT_WINDOW_HEIGHT);glutPostRedisplay();}
void mouse(int b,int state,int px,int py){if(b==GLUT_LEFT_BUTTON){dragging=state==GLUT_DOWN;if(dragging)pos(px,py);}}
void motion(int px,int py){if(dragging)pos(px,py);}
int main(int argc,char** argv){glutInit(&argc,argv);glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);glutInitWindowSize(600,400);glutCreateWindow("Q16 - Click-and-Drag Square");glClearColor(0,0,0,1);glutDisplayFunc(display);glutMouseFunc(mouse);glutMotionFunc(motion);glutMainLoop();}
