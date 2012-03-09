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

const char rendername[4][20]  = {"normal","antialiasing","depth of filed","motion blur"};

int rendermode = NORMAL;


#define VPASSES 10

#define STEPLENGTH 0.01
#define MAXWEIGHT 0.3
#define MPASSES 10

#define JITTER 0.001

#define DPASSES 20
#define JITTERMODEL 0.01

void set_lights();

const float jittermodel[] = {0.01,0.02,0.03,0.04};

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
    //printf("%s\n",vs);
    //printf("%s\n",fs);
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
	//~ set_lights();
	gluLookAt(eye.x,eye.y,eye.z,view.x,view.y,view.z,up.x,up.y,up.z);
	set_lights();

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
    float light0_diffuse[] = {0.6,0.6,0.6,1.0};
    float light0_specular[] = {0.6,0.6,0.6,1.0};
    float light0_position[] = {-2.0,2.0,3.2,1.0};
    float light0_direction[] = {2.0,-2.0,-3.2,1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);

    glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
    glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
    glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light0_direction);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void do_filllights(){
    float light1_ambient[] = {0.0,0.0,0.0,0.0};
    float light1_diffuse[] = {0.3,0.3,0.3,1.0};
    float light1_specular[] = {0.3,0.3,0.3,1.0};
    float light1_position[] = {2.0,1.5,3.2,1.0};
    float light1_direction[] = {-2.0,-1.5,-3.2,1.0};

    glLightfv(GL_LIGHT1,GL_AMBIENT,light1_ambient);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,light1_diffuse);
    glLightfv(GL_LIGHT1,GL_SPECULAR,light1_specular);
    glLightfv(GL_LIGHT1,GL_POSITION,light1_position);
    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,light1_direction);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
}

void do_backlights(){
    float light2_ambient[] = {0.0,0.0,0.0,0.0};
    float light2_diffuse[] = {0.3,0.3,0.3,1.0};
    float light2_specular[] = {0.4,0.4,0.4,1.0};
    float light2_position[] = {0.0,1.5,-1.5,1.0};
    float light2_direction[] = {0.0,-1.5,1.5,1.0};

    glLightfv(GL_LIGHT2,GL_AMBIENT,light2_ambient);
    glLightfv(GL_LIGHT2,GL_DIFFUSE,light2_diffuse);
    glLightfv(GL_LIGHT2,GL_SPECULAR,light2_specular);
    glLightfv(GL_LIGHT2,GL_POSITION,light2_position);
    glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,light2_direction);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT2);
}



void do_material(){	
    float mat_ambient[] = {0.0,0.0,0.0,0.0};
    float mat_diffuse[] = {1.0,0.8,0.0,1.0};
    float mat_specular[] = {0.6,0.6,0.6,1.0};
    float mat_shininess[] = {9.0};

    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}

void set_lights(){
	do_keylights();
    do_filllights();
    do_backlights();
    do_material();
}

//load plyModel
const char* filename = "bunny.ply";
//const char* filename = "dragon.ply";
plyModel bunny(filename);


//end

void draw_bunny()
{
	glPushMatrix();
    glTranslatef(0.03,-0.1,0.0);
    bunny.Draw();
    glPopMatrix();
}

void draw_stuff()
{
	glClearColor(0.0,0.0,0.0,0.0);
    glEnable(GL_DEPTH_TEST);

    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
	draw_bunny();
    
    //glutSolidTorus(0.4,0.8,128,128);
    
}



void jitter_view()
{
    view.x = JITTER*genrand();
    view.y = JITTER*genrand();
    view.z = JITTER*genrand();
}
void jitter_model(float jitter)
{
	glTranslatef(jitter*genrand(),jitter*genrand(),jitter*genrand());
}

void reset_view()
{
	view.x = 0.0;
	view.y = 0.0;
	view.z = 0.0;
}

void render()
{
	//~ double modelview[16];
	
	//~ glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	//~ printf("\n");
	//~ for(int i = 0;i < 16; i++)
	//~ {
		//~ printf("%f\t",modelview[i]);
	//~ }
	//~ printf("end\n");
	int view_pass;
	int step;
	//~ set_lights();
	setup_the_viewvolume(eye,view,up);
	//set_lights();
	switch(rendermode){
		case NORMAL:
		reset_view();
		//~ setup_the_viewvolume(eye,view,up);
		//~ set_lights();
		draw_stuff();
		break;
		
		case AA:
		reset_view();
		glClear(GL_ACCUM_BUFFER_BIT);
		for(view_pass=0;view_pass<VPASSES;view_pass++){
			jitter_view();
			setup_the_viewvolume(eye,view,up);
			//~ set_lights();
			draw_stuff();
			glFlush();
			glAccum(GL_ACCUM,1.0/(float)(VPASSES));
		}
		glAccum(GL_RETURN,1.0);
		break;
		
		case FIELD:
		reset_view();
		//~ setup_the_viewvolume(eye,view,up);
		//~ set_lights();
		draw_stuff();
		glClear(GL_ACCUM_BUFFER_BIT);
		for(view_pass=0;view_pass<DPASSES;view_pass++){
			glPushMatrix();
			jitter_model(jittermodel[0]);
			glTranslatef(-0.05,0.05,-0.1);
			//glAccum(GL_MULT,0.5);
			draw_stuff();
			glPopMatrix();
			glPushMatrix();
			jitter_model(jittermodel[1]);
			glTranslatef(0.05,0.1,-0.2);
			//glAccum(GL_MULT,0.5);
			draw_bunny();
			glPopMatrix();
			glAccum(GL_ACCUM,1.0/(float)(DPASSES));
		}
		glAccum(GL_RETURN,1.0);
		draw_bunny();
		break;
		
		case MOTIONBLUR:
		reset_view(); 
		//~ setup_the_viewvolume(eye,view,up);
		//~ set_lights();
		glPushMatrix();
		glTranslatef(-STEPLENGTH*MPASSES,0.0,0.0);
		glClear(GL_ACCUM_BUFFER_BIT);
		for(step=0;step<MPASSES;step++){
			glAccum(GL_MULT,(float)step/MPASSES+0.2);
			glTranslatef(STEPLENGTH,0.0,0.0);
			draw_stuff();
			glAccum(GL_ACCUM,(1.0-(float)MAXWEIGHT)/(float)(MPASSES));
		}
		draw_stuff();
		glAccum(GL_ACCUM,MAXWEIGHT);
		glAccum(GL_RETURN,1.0);
		draw_bunny();
		glPopMatrix();
		reset_view();
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
		printf("%d:%s\n",rendermode,rendername[rendermode-1]);
	} else {
		rendermode = 0;
	}
}

void createGLUTMenus() {


	glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Normal",NORMAL);
	glutAddMenuEntry("Antialiasing",AA);
	glutAddMenuEntry("DepthofField",FIELD);
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
    //~ set_lights();
    glslprogram = set_shaders();
    glutSpecialFunc(skey);
    glutKeyboardFunc(key);
    glutDisplayFunc(render);
	createGLUTMenus();
    glutMainLoop();
    return 0;
}
