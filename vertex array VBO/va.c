#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>

struct point {
	float x,y,z;
};

void setup_the_viewlolume(){
	struct point eye, view, up;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,1.0,0.1,20.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	eye.x = 2.0;
	eye.y = 2.0;
	eye.z = 2.0;

	view.x = 0.0;
	view.y = 0.0;
	view.z = 0.0;

	up.x = 0.0;
	up.y = 1.0;
	up.z = 0.0;

	gluLookAt(eye.x,eye.y,eye.z,view.x,view.y,view.z,up.x,up.y,up.z);

}
    GLfloat vertices[] = {
    0.0,0.0,0.0, //1.0,0.0,0.0,
    0.0,1.0,0.0, //0.0,1.0,0.0,
    1.0,1.0,0.0, //0.0,0.0,1.0,
    1.0,0.0,0.0, //1.0,0.0,0.0,
    0.0,0.0,1.0, //0.0,1.0,0.0,
    0.0,1.0,1.0, //0.0,0.0,1.0,
    1.0,1.0,1.0, //1.0,1.0,0.0,
    1.0,0.0,1.0, //0.8,0.8,0.8
    };

    GLubyte face[] = {
    4,7,6,5,
    0,3,2,1,
    3,2,6,7,
    1,0,4,5,
    5,6,2,1,
    3,7,4,0
    };

    GLfloat normal[6][3] = {
    0.0,0.0,1.0,
    0.0,0.0,-1.0,
    1.0,0.0,0.0,
    -1.0,0.0,0.0,
    0.0,1.0,0.0,
    0.0,-1.0,0.0
    };
void draw_stuff()
{
    int i;



    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //glDrawElements(GL_QUADS,24,GL_UNSIGNED_BYTE,face);
    for(i=0;i<6;i++){
        glNormal3fv(normal[i]);
        glDrawElements(GL_QUADS,4,GL_UNSIGNED_BYTE,NULL+4*i);

    }
    glFlush();
}

void do_lights(){
    float light0_ambient[] = {0.0,0.0,0.0,0.0};
    float light0_diffuse[] = {1.0,1.0,1.0,0.0};
    float light0_specular[] = {1.0,1.0,1.0,0.0};
    float light0_position[] = {1.5,2.0,2.0,1.0};
    float light0_direction[] = {-1.5,-2.0,-2.0,1.0};

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);

    glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
    glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,1.0);
    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,180.0);
    glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,0.5);
    glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.1);
    glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.01);
    glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
    glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light0_direction);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void do_material(){
    float mat_ambient[] = {0.0,0.0,0.0,0.0};
    float mat_diffuse[] = {0.9,0.9,0.1,1.0};
    float mat_specular[] = {1.0,1.0,1.0,1.0};
    float mat_shininess[] = {2.0};

    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}

int mybuf[2] = {1,2};
void initOGL(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize(512,512);
    glutInitWindowPosition(100,50);
    glutCreateWindow("my_cube");
    glClearColor(0.35,0.35,0.35,0.0);
    glEnable(GL_DEPTH_TEST);
    setup_the_viewlolume();
    do_lights();
    do_material();
    glBindBuffer(GL_ARRAY_BUFFER,mybuf[0]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexPointer(3,GL_FLOAT,3*sizeof(GLfloat),0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mybuf[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(face),face,GL_STATIC_DRAW);
}
int main(int argc,char** argv)
{
    initOGL(argc,argv);
    glutDisplayFunc(draw_stuff);
    glutMainLoop();
    return 0;
}
