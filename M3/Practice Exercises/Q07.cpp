#include <GL/freeglut.h>
void display() {
    glClear(GL_COLOR_BUFFER_BIT); glColor3f(1,1,1);
    glPushMatrix(); glTranslatef(-0.3f,-0.15f,0); glScalef(0.003f,0.003f,1);
    for (const char* s="HI"; *s; ++s) glutStrokeCharacter(GLUT_STROKE_ROMAN, *s);
    glPopMatrix(); glutSwapBuffers();
}
int main(int argc,char** argv) {
    glutInit(&argc,argv); glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB); glutInitWindowSize(600,400);
    glutCreateWindow("Q07 - Stroke Font Practice"); glClearColor(0,0,0,1); glutDisplayFunc(display); glutMainLoop();
}
