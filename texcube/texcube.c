#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glut.h>
static GLdouble spin = 0.5;
struct point {
	float x,y,z;
};

void setup_the_viewlolume(){
	struct point eye, view, up;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,1.0,0.1,20.0);
	//glViewport(0,0,200,200);

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


void draw_stuff(){
    int i;
    struct point front[4] = {
    {0.0,0.0,1.0},{0.0,1.0,1.0},{1.0,1.0,1.0},{1.0,0.0,1.0}
    };
    struct point back[4] = {
    {1.0,0.0,0.0},{1.0,1.0,0.0},{0.0,1.0,0.0},{0.0,0.0,0.0}
    };
    struct point left[4] = {
    {0.0,0.0,0.0},{0.0,1.0,0.0},{0.0,1.0,1.0},{0.0,0.0,1.0}
    };
    struct point right[4] = {
    {1.0,0.0,1.0},{1.0,1.0,1.0},{1.0,1.0,0.0},{1.0,0.0,0.0}
    };
    struct point top[4] = {
    {0.0,1.0,1.0},{0.0,1.0,0.0},{1.0,1.0,0.0},{1.0,1.0,1.0}
    };
    struct point bottom[4] = {
    {0.0,0.0,0.0},{0.0,0.0,1.0},{1.0,0.0,1.0},{1.0,0.0,0.0}
    };

    glClearColor(0.35,0.35,0.35,0.0);
    float t = 1.0;
    float mytexcoords[4][2] = {{0.0,0.0},{0.0,t},{t,t},{t,0.0}};
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    glColor3f(1.0,1.0,0.0);

    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,1);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);

    glNormal3f(0.0,0.0,1.0);
    for(i=0;i<4;i++) {
		glTexCoord2fv(mytexcoords[i]);
		glVertex3f(front[i].x,front[i].y,front[i].z);
	}
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
	glBegin(GL_QUADS);
    glNormal3f(0.0,0.0,-1.0);
    for(i=0;i<4;i++) glVertex3f(back[i].x,back[i].y,back[i].z);
    glNormal3f(-1.0,0.0,0.0);
    for(i=0;i<4;i++) glVertex3f(left[i].x,left[i].y,left[i].z);
    glNormal3f(1.0,0.0,0.0);
    for(i=0;i<4;i++) glVertex3f(right[i].x,right[i].y,right[i].z);
    glNormal3f(0.0,1.0,0.0);
    for(i=0;i<4;i++) glVertex3f(top[i].x,top[i].y,top[i].z);
    glNormal3f(0.0,-1.0,0.0);
    for(i=0;i<4;i++) glVertex3f(bottom[i].x,bottom[i].y,bottom[i].z);
    glEnd();
    //glPopMatrix();
    glFlush();
    //glutSwapBuffers();
    //glutPostRedisplay();
}

void do_lights(){
    float light0_ambient[] = {0.0,0.0,0.0,0.0};
    float light0_diffuse[] = {2.0,2.0,2.0,0.0};
    float light0_specular[] = {2.25,2.25,2.25,0.0};
    float light0_position[] = {1.5,2.0,2.0,1.0};
    float light0_direction[] = {-1.5,-2.0,-2.0,1.0};

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);
    glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
    glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,1.0);
    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,180.0);
    glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1.0);
    glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.2);
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

void load_texture(char *filename)
{
	FILE *fptr;
	char buf[512];
	char *parse;
	int im_size,im_width,im_height,max_color;
	unsigned char *texture_bytes;
	
	fptr = fopen(filename,"r");
	fgets(buf,512,fptr);
	do{
		fgets(buf,512,fptr);
	} while(buf[0] == '#');
	parse = strtok(buf," \t"); 
	im_width = atoi(parse);
	
	parse = strtok(NULL," \n");
	im_height = atoi(parse);
	
	fgets(buf,512,fptr);
	parse = strtok(buf," \n");
	max_color = atoi(parse);
	
	im_size = im_width*im_height;
	texture_bytes = (unsigned char *)calloc(3,im_size);
	fread(texture_bytes,3,im_size,fptr);
	fclose(fptr);
	printf("%d %d %d\n",im_width,im_height,im_size);
	//printf("%s",texture_bytes);
	glBindTexture(GL_TEXTURE_2D,1);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,im_width,im_height,0,GL_RGB,GL_UNSIGNED_BYTE,texture_bytes);

	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	free(texture_bytes);
}

int main(int argc, char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize(640,640);
    glutInitWindowPosition(100,50);
    glutCreateWindow("my_tex_cube");
    glEnable(GL_DEPTH_TEST);
    load_texture(argv[1]);
    setup_the_viewlolume();
    do_lights();
    do_material();
    glutDisplayFunc(draw_stuff);
    glutMainLoop();
    return 0;
}




















