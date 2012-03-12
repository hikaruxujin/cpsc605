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

float eye[3] = {3.0,3.0,3.0};
float viewpt[3] = {0.0,0.0,0.0};
float up[3] = {0.0,1.0,0.0};
float light0_position[] = {3.0,3.0,3.0,1.0};

void set_light()
{
	glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
}
 

void set_material()
{
	float mat_diffuse[] = {1.0,0.8,0.0,1.0};
	float mat_specular[] = {0.6,0.6,0.6,1.0};
	float mat_shininess[] = {20.0};
	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}

void view_volume()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,1.0,1.0,20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0],eye[1],eye[2],viewpt[0],viewpt[1],viewpt[2],up[0],up[1],up[2]);
}

void renderScene(void)
{
	glClearColor(0.5,0.4,0.3,1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,1);
	glPushMatrix();
	glRotatef(-35.0,1.0,0.0,0.0);
	//glColor3f(0.5,0.1,0.1);
	glutSolidTeapot(1.0);
	glPopMatrix();
	glutSwapBuffers();
}

GLuint set_shaders()
{
    GLint vertComplied, fragComplied;
    char *vs, *fs;
    GLuint v,f,p;

    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
    vs = read_shader_program("phong.vert");
    fs = read_shader_program("phong.frag");
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
	glUniform1i(location,0);
}
	
void load_texture(char *filename)
{
	printf("%s\n",filename);
	FILE *fptr;
	unsigned char buf[512];
	int im_size,im_width,im_height,max_color;
	unsigned char *texture_bytes,*parse;
	
	fptr = fopen(filename,"r");
	fgets(buf,512,fptr);
	printf("%s\n",buf);
	do{
		fgets(buf,512,fptr);
	} while(buf[0] == '#');
	parse = strtok(buf," \t");
	im_width = atoi(parse);
	
	parse = strtok(buf," \n");
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
	cfree(texture_bytes);
}

int main(int argc,char** argv)
{
	int p;
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);
    glutCreateWindow("phong shaded teapot");
    load_texture(argv[1]);
    glEnable(GL_DEPTH_TEST);
    view_volume();
    set_light();
    set_material();
    p = set_shaders();
    set_uniform(p);
    glutDisplayFunc(renderScene);
    //glutIdleFunc(renderScene);
    glutMainLoop();
    return 0;
}
