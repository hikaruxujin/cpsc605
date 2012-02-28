#define GL_GLEXT_PROTOTYPES 1
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <malloc.h>
#include <cmath>
#include <string>
#include <fcntl.h>
#include "plyModel.h"


#define NORMAL 1
#define AA 2
#define FIELD 3
#define MOTIONBLUR 4

int rendermode = NORMAL;


#define VPASSES 10
#define JITTER 0.001

double genrand()
{
    return (((double)(rand()+1))/2147483649.);
}


int glslprogram = 0;
//glsl begin

char *read_shader_program(const char* filename)
{
    FILE *fp;
    char *content = NULL;
    int fd,count;
    fd = open(filename,O_RDONLY);
    count = lseek(fd,0,SEEK_END);
    close(fd);
    content = (char*)calloc(sizeof(char),(count+1));
    fp = fopen(filename,"r");
    count = fread(content,sizeof(char),count,fp);
    content[count] = '\0';
    fclose(fp);
    return content;
}

GLuint set_shaders()
{
    char *vs, *fs;
    GLuint v,f,p;

    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
    //~ vs = read_shader_program("phong.vert");
    //~ fs = read_shader_program("phong.frag");
    vs = read_shader_program("blinn.vert");
    fs = read_shader_program("blinn.frag");
    printf("%s\n",vs);
    printf("%s\n",fs);
    glShaderSource(v,1,(const char **)&vs,NULL);
    glShaderSource(f,1,(const char **)&fs,NULL);
    free(vs);
    free(fs);
    glCompileShader(v);
    glCompileShader(f);
    p = glCreateProgram();
    glAttachShader(p,f);
    glAttachShader(p,v);
    glLinkProgram(p);
    glUseProgram(p);
    printf("shader is OK%d\n",p);
    return (p);
}

//glsl end

struct point {
	float x,y,z;
};

struct point eye = {0.0,0.0,0.4};
struct point view = {0.0,0.0,0.0};
struct point up = {0.0,1.0,0.0};

float angleH = 0.0;
float angleV = 0.0;
float distance = 0.4;
#define PI 3.14159265

void setup_the_viewvolume(point& eye,point& view,point& up){
	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,1.0,0.1,20.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();



	gluLookAt(eye.x,eye.y,eye.z,view.x,view.y,view.z,up.x,up.y,up.z);

}


void key(unsigned char key, int x, int y)
{
  if (key == '\033')
    exit(0);
    if(key == ' ')
  {
	  angleH = 0.0;
	  angleV = 0.0;
	  distance = 0.4;
	  eye.z = distance*cos(angleH);
	  eye.x = distance*sin(angleH);
	  eye.y = distance*sin(angleV);
	  glutPostRedisplay();
  }
  if(key == 'Z')
  {
	  distance -= 0.05;
	  eye.z = distance*cos(angleH);
	  eye.x = distance*sin(angleH);
	  eye.y = distance*sin(angleV);
	  glutPostRedisplay();
  }
  if(key == 'z')
  {
	  distance += 0.05;
	  eye.z = distance*cos(angleH);
	  eye.x = distance*sin(angleH);
	  eye.y = distance*sin(angleV);
	  glutPostRedisplay();
  }
  printf("%c %d %d\n",key,x,y);
}

void skey(int key, int x, int y)
{
  if (key == GLUT_KEY_LEFT)
  {
	  angleH += 0.1;
	  if(angleH > 2 * PI)
	  {
		  angleH = 0.0;
	  }
	  eye.z =distance*cos(angleH);
	  eye.x =distance*sin(angleH);
	  glutPostRedisplay();
  }
  if(key == GLUT_KEY_RIGHT)
  {
	  angleH -= 0.1;
	  if(angleH < 0.0)
	  {
		  angleH = 2 * PI;
	  }
	  eye.z =distance*cos(angleH);
	  eye.x =distance*sin(angleH);
	  glutPostRedisplay();
  }
  if(key == GLUT_KEY_UP)
  {
	  angleV += 0.1;
	  if(angleV > 2 * PI)
	  {
		  angleV = 0.0;
	  }
	  eye.y =distance*sin(angleV);
	  glutPostRedisplay();
  }
    if(key == GLUT_KEY_DOWN)
  {
	  angleV -= 0.1;
	  if(angleV < 0.0)
	  {
		  angleV = 2 * PI;
	  }
	  eye.y =distance*sin(angleV);
	  glutPostRedisplay();
  }
  printf("%d %d %d\n",key,x,y);
}

void do_keylights(){
    float light0_ambient[] = {0.0,0.0,0.0,0.0};
    float light0_diffuse[] = {0.8,0.8,0.8,0.0};
    float light0_specular[] = {0.8,0.8,0.8,0.0};
    float light0_position[] = {-2.0,2.0,2.0,1.0};
    float light0_direction[] = {2.0,-2.0,-2.0,1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);

    glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
    glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,1.0);
    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,180.0);
    glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1.0);
    glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.01);
    glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.01);
    glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
    glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light0_direction);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void do_filllights(){
    float light1_ambient[] = {0.0,0.0,0.0,0.0};
    float light1_diffuse[] = {0.4,0.4,0.4,0.0};
    float light1_specular[] = {0.4,0.4,0.4,0.0};
    float light1_position[] = {2.0,0.0,2.0,1.0};
    float light1_direction[] = {2.0,0.0,-2.0,1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light1_ambient);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);

    glLightfv(GL_LIGHT1,GL_AMBIENT,light1_ambient);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,light1_diffuse);
    glLightfv(GL_LIGHT1,GL_SPECULAR,light1_specular);
    glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,1.0);
    glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,180.0);
    glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION,3.5);
    glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION,0.01);
    glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,0.01);
    glLightfv(GL_LIGHT1,GL_POSITION,light1_position);
    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,light1_direction);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
}

void do_backlights(){
    float light2_ambient[] = {0.0,0.0,0.0,0.0};
    float light2_diffuse[] = {0.3,0.3,0.3,0.0};
    float light2_specular[] = {0.3,0.3,0.3,0.0};
    float light2_position[] = {0.0,3.0,-3.0,1.0};
    float light2_direction[] = {0.0,-2.0,2.0,1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light2_ambient);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);

    glLightfv(GL_LIGHT2,GL_AMBIENT,light2_ambient);
    glLightfv(GL_LIGHT2,GL_DIFFUSE,light2_diffuse);
    glLightfv(GL_LIGHT2,GL_SPECULAR,light2_specular);
    glLightf(GL_LIGHT2,GL_SPOT_EXPONENT,2.5);
    glLightf(GL_LIGHT2,GL_SPOT_CUTOFF,180.0);
    glLightf(GL_LIGHT2,GL_CONSTANT_ATTENUATION,1.0);
    glLightf(GL_LIGHT2,GL_LINEAR_ATTENUATION,0.01);
    glLightf(GL_LIGHT2,GL_QUADRATIC_ATTENUATION,0.01);
    glLightfv(GL_LIGHT2,GL_POSITION,light2_position);
    glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,light2_direction);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT2);
}



void do_material(){	
    float mat_ambient[] = {0.0,0.0,0.0,0.0};
    float mat_diffuse[] = {1.0,0.8,0.0,1.0};
    float mat_specular[] = {1.0,1.0,1.0,1.0};
    float mat_shininess[] = {14.0};

    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}

//load plyModel
const char* filename = "bunny.ply";
//const char* filename = "dragon.ply";
plyModel bunny(filename);


//end

void draw_stuff()
{
	glClearColor(0.0,0.0,0.0,0.0);
    glEnable(GL_DEPTH_TEST);
    
	setup_the_viewvolume(eye,view,up);
	do_keylights();
    do_filllights();
    do_backlights();
    do_material();
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    glTranslatef(0.03,-0.1,0.0);
    bunny.Draw();
    glPopMatrix();
    
    //glutSolidTorus(0.4,0.8,128,128);
    
}

void jitter_view()
{
    view.x = JITTER*genrand();
    view.y = JITTER*genrand();
    view.z = JITTER*genrand();
}

void reset_view()
{
	view.x = 0.0;
	view.y = 0.0;
	view.z = 0.0;
}

void render()
{
	int view_pass;
	switch(rendermode){
		case NORMAL:
		reset_view();
		draw_stuff();
		break;
		
		case AA:
		glClear(GL_ACCUM_BUFFER_BIT);
		for(view_pass=0;view_pass<VPASSES;view_pass++){
			jitter_view();
			draw_stuff();
			glFlush();
			glAccum(GL_ACCUM,1.0/(float)(VPASSES));
		}
		glAccum(GL_RETURN,1.0);
		break;
		
		case FIELD:
		break;
		
		case MOTIONBLUR:
		break;
	}
	glutSwapBuffers();
	
}

//menu

void processMenuEvents(int option) {
	if(option > 0 && option <=5 )
	{
		rendermode = option;
		glutPostRedisplay();
		printf("%d\n",rendermode);
	} else {
		rendermode = 0;
	}
}

void createGLUTMenus() {

	int submenu;

	submenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Normal",NORMAL);
	glutAddMenuEntry("Antialiasing",AA);
	glutAddMenuEntry("FieldDepth",FIELD);
	glutAddMenuEntry("MotionBlur",MOTIONBLUR);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
//end menu

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE|GLUT_ACCUM);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(100,50);
    glutCreateWindow("Golden Bunny");

    //setup_the_viewvolume();

    //open shaders
    glslprogram = set_shaders();
    glutSpecialFunc(skey);
    glutKeyboardFunc(key);
    glutDisplayFunc(render);
	createGLUTMenus();
    glutMainLoop();
    return 0;
}
