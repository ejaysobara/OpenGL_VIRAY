#include <GL/freeglut.h>
#include <cstdio>
int mouseX=0, mouseY=0; bool dragging=false;
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if(dragging) { int w=glutGet(GLUT_WINDOW_WIDTH), h=glutGet(GLUT_WINDOW_HEIGHT); float x=2.0f*mouseX/w-1, y=1-2.0f*mouseY/h; char s[80]; std::snprintf(s,sizeof s,"OpenGL (%.2f, %.2f)",x,y); glColor3f(1,1,1); glRasterPos2f(-0.95f,0.85f); for(char* p=s;*p;++p)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*p); }
    glutSwapBuffers();
}
void mouse(int,int state,int x,int y){dragging=state==GLUT_DOWN;mouseX=x;mouseY=y;glutPostRedisplay();}
void motion(int x,int y){mouseX=x;mouseY=y;glutPostRedisplay();}
int main(int argc,char** argv){glutInit(&argc,argv);glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);glutInitWindowSize(600,400);glutCreateWindow("Q09 - Active Motion Coordinate Display");glClearColor(0,0,0,1);glutDisplayFunc(display);glutMouseFunc(mouse);glutMotionFunc(motion);glutMainLoop();}
