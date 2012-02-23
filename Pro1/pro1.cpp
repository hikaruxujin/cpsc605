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
 
//plyModel begin
struct normal{
	 float x;
	 float y;
	 float z;
};
 
class plyModel 
{
public:
	int Load(const char *filename);
	void Draw();
	normal calculateNormal( float *coord1, float *coord2, float *coord3 );
	plyModel();
	plyModel(const char* filename);
	
	~plyModel()
	{
		free(Triangles);
		free(Vertices);
		free(Normals);
		printf("free model\n");
	}
 private:
    float* Triangles;
	float* Vertices;
	float* Normals;
 
	int TotalConnectedPoints;
	int TotalFaces;
	plyModel(const plyModel&);
	plyModel& operator=(const plyModel&);
 
};
 
 
 
plyModel::plyModel():
	Triangles(NULL),
	Vertices(NULL),
	Normals(NULL),
	TotalConnectedPoints(0),
	TotalFaces(0)
{
	
}

plyModel::plyModel(const char* filename):
	Triangles(NULL),
	Vertices(NULL),
	Normals(NULL),
	TotalConnectedPoints(0),
	TotalFaces(0)
{
    Load(filename);
}

 
 
normal plyModel::calculateNormal( float *coord1, float *coord2, float *coord3 )
{
   /* calculate Vector1 and Vector2 */
   float va[3], vb[3], vr[3], val;
   va[0] = coord1[0] - coord2[0];
   va[1] = coord1[1] - coord2[1];
   va[2] = coord1[2] - coord2[2];
 
   vb[0] = coord1[0] - coord3[0];
   vb[1] = coord1[1] - coord3[1];
   vb[2] = coord1[2] - coord3[2];
 
   /* cross product */
   vr[0] = va[1] * vb[2] - vb[1] * va[2];
   vr[1] = vb[0] * va[2] - va[0] * vb[2];
   vr[2] = va[0] * vb[1] - vb[0] * va[1];
 
   /* normalization factor */
   val = sqrt( vr[0]*vr[0] + vr[1]*vr[1] + vr[2]*vr[2] );
 
	normal norm;
	norm.x = vr[0]/val;
	norm.y = vr[1]/val;
	norm.z = vr[2]/val;
 
 
	return norm;
}
 
 
 
int plyModel::Load(const char* filename)
{

	this->TotalConnectedPoints = 0;
	this->TotalFaces = 0;
 
    char* pch = strstr(filename,".ply");
 
    if (pch != NULL)
    {
		//open file
		FILE* file = fopen(filename,"r");
 
		if (file)
		{
			int i = 0;   
            int triangle_index = 0;
			int normal_index = 0;
			char buffer[1000];
 
 
			fgets(buffer,300,file);			// ply
 
 
			// READ HEADER
			// -----------------
 
			// Find number of vertexes
			while (  strncmp( "element vertex", buffer,strlen("element vertex")) != 0  )
			{
				fgets(buffer,300,file);			// format
			}
			strcpy(buffer, buffer+strlen("element vertex"));
			sscanf(buffer,"%i", &this->TotalConnectedPoints);
 
 
			// Find number of vertexes
			fseek(file,0,SEEK_SET);
			while (  strncmp( "element face", buffer,strlen("element face")) != 0  )
			{
				fgets(buffer,300,file);			// format
			}
			strcpy(buffer, buffer+strlen("element face"));
			sscanf(buffer,"%i", &this->TotalFaces);
 
 
			// go to end_header
			while (  strncmp( "end_header", buffer,strlen("end_header")) != 0  )
			{
				fgets(buffer,300,file);			// format
			}
 
			//----------------------
			Vertices = (float*) malloc (3*TotalConnectedPoints*sizeof(float));
			Triangles = (float*) malloc(9*TotalFaces*sizeof(float));
			Normals  = (float*) malloc(9*TotalFaces*sizeof(float));
 
			// read verteces
			i =0;
			for (int iterator = 0; iterator < this->TotalConnectedPoints; iterator++)
			{
				fgets(buffer,300,file);
 
				sscanf(buffer,"%f %f %f", &Vertices[i], &Vertices[i+1], &Vertices[i+2]);
				i += 3;
			}
 
			// read faces
			i =0;
			for (int iterator = 0; iterator < this->TotalFaces; iterator++)
			{
				fgets(buffer,300,file);
 
					if (buffer[0] == '3')
					{
 
						int vertex1 = 0, vertex2 = 0, vertex3 = 0;
						//sscanf(buffer,"%i%i%i\n", vertex1,vertex2,vertex3 );
						buffer[0] = ' ';
						sscanf(buffer,"%i%i%i", &vertex1,&vertex2,&vertex3 );
						
						//printf("%f %f %f ", Vertices[3*vertex1], Vertices[3*vertex1+1], Vertices[3*vertex1+2]);
 
						Triangles[triangle_index] = Vertices[3*vertex1];
						Triangles[triangle_index+1] = Vertices[3*vertex1+1];
						Triangles[triangle_index+2] = Vertices[3*vertex1+2];
						Triangles[triangle_index+3] = Vertices[3*vertex2];
						Triangles[triangle_index+4] = Vertices[3*vertex2+1];
						Triangles[triangle_index+5] = Vertices[3*vertex2+2];
						Triangles[triangle_index+6] = Vertices[3*vertex3];
						Triangles[triangle_index+7] = Vertices[3*vertex3+1];
						Triangles[triangle_index+8] = Vertices[3*vertex3+2];
 
						float coord1[3] = { Triangles[triangle_index], Triangles[triangle_index+1],Triangles[triangle_index+2]};
						float coord2[3] = {Triangles[triangle_index+3],Triangles[triangle_index+4],Triangles[triangle_index+5]};
						float coord3[3] = {Triangles[triangle_index+6],Triangles[triangle_index+7],Triangles[triangle_index+8]};
						normal norm = this->calculateNormal( coord1, coord2, coord3 );
 
						Normals[normal_index] = norm.x;
						Normals[normal_index+1] = norm.y;
						Normals[normal_index+2] = norm.z;
						Normals[normal_index+3] = norm.x;
						Normals[normal_index+4] = norm.y;
						Normals[normal_index+5] = norm.z;
						Normals[normal_index+6] = norm.x;
						Normals[normal_index+7] = norm.y;
						Normals[normal_index+8] = norm.z;
 
						normal_index += 9;
 
						triangle_index += 9;
					}
 
 
					i += 3;
			}
 
 
			fclose(file);
		} else { printf("File can't be opened\n"); 
		}
    } else {
      printf("File does not have a .PLY extension. ");    
    }   
	return 0;
}
 
void plyModel::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);	
 	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3,GL_FLOAT,	0,Triangles);	
	glNormalPointer(GL_FLOAT, 0, Normals);
	glDrawArrays(GL_TRIANGLES, 0, 3*TotalFaces);	
	glDisableClientState(GL_VERTEX_ARRAY);    
	glDisableClientState(GL_NORMAL_ARRAY);
}
//plyModel end


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
    //vs = read_shader_program("phong.vert");
    //fs = read_shader_program("phong.frag");
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

void setup_the_viewlolume(){
	struct point eye, view, up;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,1.0,0.1,20.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	eye.x = 0.0;
	eye.y = 0.0;
	eye.z = 0.4;

	view.x = 0.0;
	view.y = 0.0;
	view.z = 0.0;

	up.x = 0.0;
	up.y = 1.0;
	up.z = 0.0;

	gluLookAt(eye.x,eye.y,eye.z,view.x,view.y,view.z,up.x,up.y,up.z);

}

//load plyModel
const char* filename = "bunny.ply";
//const char* filename = "dragon.ply";
plyModel bunny(filename);


//end

void draw_stuff()
{

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    //glPushMatrix();
    //glTranslatef(-0.2,0.1,0.0);
    //bunny.Draw();
    //glPopMatrix();
    //glPushMatrix();
    //glTranslatef(0.2,0.1,0.0);
    //bunny.Draw();
    //glPopMatrix();
    //glPushMatrix();
    //glTranslatef(0.2,-0.3,0.0);
    //bunny.Draw();
    //glPopMatrix();
    //glPushMatrix();
    //glTranslatef(-0.2,-0.3,0.0);
    //bunny.Draw();
    //glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.03,-0.1,0.0);
    bunny.Draw();
    glPopMatrix();
    
    //glutSolidTorus(0.4,0.8,128,128);
    
    glutSwapBuffers();
}

void do_lights(){
    float light0_ambient[] = {0.0,0.0,0.0,0.0};
    float light0_diffuse[] = {0.5,0.5,0.5,0.0};
    float light0_specular[] = {0.5,0.5,0.5,0.0};
    float light0_position[] = {0.0,-0.5,-0.5,1.0};
    float light0_direction[] = {0.0,1.0,1.0,1.0};

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
    float mat_diffuse[] = {0.5,0.5,0.5,1.0};
    float mat_specular[] = {1.0,1.0,1.0,1.0};
    float mat_shininess[] = {5.0};

    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
    glutInitWindowSize(512,512);
    glutInitWindowPosition(100,50);
    glutCreateWindow("my bunny");
    glClearColor(0.0,0.0,0.0,0.0);
    glEnable(GL_DEPTH_TEST);
    setup_the_viewlolume();
    do_lights();
    do_material();
    //set_shaders();
    glutDisplayFunc(draw_stuff);
    glutMainLoop();
    return 0;
}