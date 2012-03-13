#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define NUM_SURFACES 3
#define XRES 768
#define YRES 768

struct surface {
	char *name;
	float v[4][3];
	float mat_ambient[4];
	float mat_diffuse[4];
	float mat_specular[4];
	float mat_shininess[1];
	
};


struct surface surface[NUM_SURFACES] = {
	"floor",
	-1.0, 0.0, -1.0,
	1.0, 0.0, -1.0,
	1.0, 0.0, 1.0,
	-1.0, 0.0, 1.0,
		0.0, 0.0, 0.0, 1.0,
		0.8, 0.4, 0.0, 1.0,
		1.0, 1.0, 1.0, 1.0,
		2.0,
	"table",
	0.0, 0.3, -0.5,
	0.5, 0.3, 0.0,
	0.0, 0.3, 0.5,
	-0.5, 0.3, 0.0,
		0.0, 0.0, 0.0, 1.0,
		0.0, 0.4, 0.15, 1.0,
		1.0, 1.0, 1.0, 1.0,
		2.0,
	"other obstacle",
	-0.25, 0.55, 0.0,
	-0.25, 0.55, -0.25,
	0.5, 0.55, -0.25,
	0.5, 0.55, 0.0,
		0.0, 0.0, 0.0, 1.0,
		0.3, 0.2, 0.65, 1.0,
		1.0, 1.0, 1.0, 1.0,
		2.0
};

GLfloat light0_position[] = {-0.5, 2.0, -0.5, 1.0};
GLfloat light0_direction[] = {0.5, -2.0, 0.5, 1.0};

void lights() 
{
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
}

void material(int sindex) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, surface[sindex].mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, surface[sindex].mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, surface[sindex].mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, surface[sindex].mat_shininess);
}

char *read_shader_program(char* filename)
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
    GLint vertComplied, fragComplied;
    char *vs, *fs;
    GLuint v,f,p;

    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
    vs = read_shader_program("gsm4.vert");
    fs = read_shader_program("gsm4.frag");
    //~ printf("%s\n",vs);
    //~ printf("%s\n",fs);
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

void set_uniform(int p)
{
	int location;
	location = glGetUniformLocation(p,"mytexture");
	glUniform1i(location,7);
}

void drawquads() 
{
	int sindex, k;
	glNormal3f(0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	for (sindex=0; sindex<NUM_SURFACES; sindex++) {
		material(sindex);
		glBegin(GL_QUADS);
		for (k=0; k<4; k++) glVertex3fv(surface[sindex].v[k]);
		glEnd();
	}
	glFlush();
}

void build_shadowmap() {
	glBindTexture(GL_TEXTURE_2D, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, XRES, YRES, 0,
			GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 1);
	glDrawBuffer(GL_NONE);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, 1, 0);
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}

void save_matrix(float *ep, float *vp) {
	glMatrixMode(GL_TEXTURE);
	glActiveTexture(GL_TEXTURE7);
	glLoadIdentity();
	glTranslatef(0.0,0.0,-0.005);
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(1.0, 1.0, 1.0);
	gluPerspective(45.0, (float)(XRES)/(float)(YRES), 0.1, 20.0);
	gluLookAt(ep[0], ep[1], ep[2], vp[0], vp[1], vp[2], 0.0, 1.0, 0.0);
}

void view_volume(float *ep, float *vp) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)(XRES)/(float)(YRES), 0.1, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(ep[0], ep[1], ep[2], vp[0], vp[1], vp[2], 0.0, 1.0, 0.0);
}

GLuint sprogram;

void do_stuff() {
	float eyepoint[3], viewpoint[3];
	int k;

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.8, 0.6, 0.62, 1.0);
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 1);
	glUseProgram(0);
	for (k=0; k<3; k++) {
		eyepoint[k] = light0_position[k];
		viewpoint[k] = light0_direction[k] + light0_position[k];
	}
	view_volume(eyepoint, viewpoint);
	drawquads();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	save_matrix(eyepoint, viewpoint);
	glUseProgram(sprogram);
	set_uniform(sprogram);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, 1);
	
	eyepoint[0] = 1.0; eyepoint[1] = 2.0; eyepoint[2] = 2.0;
	viewpoint[0] = 0.0; viewpoint[1] = 0.0; viewpoint[2] = 0.0;
	view_volume(eyepoint, viewpoint);
	drawquads();
	glutSwapBuffers();	

}



int main (int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(XRES,YRES);
    glutInitWindowPosition(100,100);
    glutCreateWindow("shadowmap");
	lights();
	//~ build_shadowmap();
	sprogram = set_shaders();
	glutDisplayFunc(do_stuff);
	glutMainLoop();
	return 0;
}
