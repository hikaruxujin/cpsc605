#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


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

#define BOX 1

GLuint set_shaders()
{
    GLint vertComplied, fragComplied;
    char *vs, *fs;
    GLuint v,f,p;

    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
    vs = read_shader_program("phong.vert");
    fs = read_shader_program("phong.frag");
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

void graphics_init()
{
	int i,j;
	glClearColor(0.7,0.0,0.0,0.0);
	glNewList(BOX,GL_COMPILE);
	for(i=0;i<100;i++){
		glBegin(GL_QUAD_STRIP);
		for(j=0;j<110;j++){
			glColor3f(0.0,0.01*j,0.01*j);
			glVertex2f(-0.5+j*0.01,-0.5+i*0.01);
			glVertex2f(-0.5+j*0.01,-0.5+(i+1)*0.01);
		}
		glEnd();
	}
	glEndList();
}

int p;
void do_stuff(){
	float twist_value;
	int location;
	location = glGetUniformLocation(p,"twist");
	for(twist_value=0.0;twist_value<=3.14;twist_value +=0.01){
		glUniform1f(location,twist_value);
		glClear(GL_COLOR_BUFFER_BIT);
		glCallList(BOX);
		glutSwapBuffers();
	}
	for(twist_value=3.14;twist_value>=0.0;twist_value -=0.01){
		glUniform1f(location,twist_value);
		glClear(GL_COLOR_BUFFER_BIT);
		glCallList(BOX);
		glutSwapBuffers();
	}
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);
    glutCreateWindow("basic GLSL demo");
    p=set_shaders();
    graphics_init();
    glutDisplayFunc(do_stuff);
    glutIdleFunc(do_stuff);
    glutMainLoop();
    return 0;
}
