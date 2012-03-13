#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <string.h>

#define NULL_A (unsigned char(*)(unsigned char))0

unsigned char wowalpha(unsigned char aarg)
{
	if(aarg == 219) return (255);
	return (0);
}

struct texture_control{
	char *name;
	int texid;
	GLint format;
	unsigned char (*alpha)(unsigned char);
	GLenum texunit;
	GLint combine;
};

struct texture_control tc[] = {
	"woodplanks.ppm",1,GL_RGB,NULL_A,GL_TEXTURE0,GL_MODULATE,
	"scuff.ppm",     2,GL_RGB,NULL_A,GL_TEXTURE1,GL_MODULATE,
	"wow.ppm",       3,GL_RGBA,wowalpha,GL_TEXTURE2,GL_DECAL,
	0
};

void load_textures()
{
	FILE *fptr;
	char buf[512];
	char *parse;
	unsigned char *texture_bytes;
	int im_size,im_width,im_height,max_color;
	unsigned char *tb_alpha, *tb_src, *tb_dst, *tb_final,aarg;
	struct texture_control *tcp;
	int i;
	
	tcp = tc;
	
	while(tcp->name){
		fptr = fopen(tcp->name,"r");
		fgets(buf,512,fptr);
		do{
			fgets(buf,512,fptr);
		}while(buf[0] == '#');
		parse = strtok(buf, " \t");
		im_width = atoi(parse);
		
		parse = strtok(NULL," \n");
		im_height = atoi(parse);
		fgets(buf,512,fptr);
		parse = strtok(buf," \n");
		max_color = atoi(parse);
		im_size = im_width * im_height;
		printf("%d %d %d\n",im_width,im_height,im_size);

		texture_bytes = (unsigned char *)calloc(3,im_size);
		fread(texture_bytes,1,3*im_size,fptr);
		fclose(fptr);
		if(tcp->format == GL_RGBA){
			tb_alpha = (unsigned char *)calloc(4,im_size);
			tb_src = texture_bytes;
			tb_dst = tb_alpha;
			for(i=0;i<im_size;i++){
				aarg = *tb_src;
				*tb_dst++ = *tb_src++;
				*tb_dst++ = *tb_src++;
				*tb_dst++ = *tb_src++;
				*tb_dst++ = (tcp->alpha)(aarg);
			}
			free(texture_bytes);
			tb_final = tb_alpha;
		} else {
			tb_final = texture_bytes;
		}
		glBindTexture(GL_TEXTURE_2D,tcp->texid);
		glTexImage2D(GL_TEXTURE_2D,0,tcp->format,im_width,im_height,0,
					tcp->format,GL_UNSIGNED_BYTE,tb_final);
		free(tb_final);
		tcp++;
	}
	tcp = tc;
	while(tcp->name){
		glActiveTexture(tcp->texunit);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,tcp->texid);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,tcp->combine);
		tcp++;
	}

}


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
    float mytexcoords[4][2] = {{0.0,t},{t,t},{t,0.0},{0.0,0.0}};
    struct texture_control *tcp;
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    glColor3f(1.0,1.0,0.0);

    glBegin(GL_QUADS);

    glNormal3f(0.0,0.0,1.0);
    for(i=0;i<4;i++) {
		tcp = tc;
		while(tcp->name){
			glMultiTexCoord2fv(tcp->texunit,mytexcoords[i]);
			tcp++;
		}		
		glVertex3f(front[i].x,front[i].y,front[i].z);
	}
	glEnd();
	tcp = tc;
	while(tcp->name){
			glActiveTexture(tcp->texunit);
			glDisable(GL_TEXTURE_2D);
			tcp++;
		}	
	
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
    float light0_diffuse[] = {1.0,1.0,1.0,0.0};
    float light0_specular[] = {1.0,1.0,1.0,0.0};
    float light0_position[] = {1.5,2.0,2.0,1.0};
    float light0_direction[] = {-1.5,-2.0,-2.0,1.0};

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



int main(int argc, char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize(640,640);
    glutInitWindowPosition(100,50);
    glutCreateWindow("multi-textured cube");
    glEnable(GL_DEPTH_TEST);
    load_textures();
    setup_the_viewlolume();
    do_lights();
    do_material();
    glutDisplayFunc(draw_stuff);
    glutMainLoop();
    return 0;
}




















