#ifndef MODEL_PLY_H
#define MODEL_PLY_H
/* Demonstrates how to load PLY files
 * Needs some refactoring.
 *
 * Model needs to be triangulated
 * Use blender
 *
 * Just the class for loading PLY files. 
 * 
 */ 
 
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <malloc.h>
#include <cmath>
#include <string>
 
 
class Model_PLY 
{
public:
	int Load(char *filename);
	void Draw();
	float* calculateNormal( float *coord1, float *coord2, float *coord3 );
	Model_PLY();
	
	~Model_PLY()
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
	Model_PLY(const Model_PLY&);
	Model_PLY& operator=(const Model_PLY&);
 
};
 
 
 
Model_PLY::Model_PLY():
	Triangles(NULL),
	Vertices(NULL),
	Normals(NULL),
	TotalConnectedPoints(0),
	TotalFaces(0)
{
 
}

 
 
float* Model_PLY::calculateNormal( float *coord1, float *coord2, float *coord3 )
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
 
	float norm[3];
	norm[0] = vr[0]/val;
	norm[1] = vr[1]/val;
	norm[2] = vr[2]/val;
 
 
	return norm;
}
 
 
 
int Model_PLY::Load(char* filename)
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
						float *norm = this->calculateNormal( coord1, coord2, coord3 );
 
						Normals[normal_index] = norm[0];
						Normals[normal_index+1] = norm[1];
						Normals[normal_index+2] = norm[2];
						Normals[normal_index+3] = norm[0];
						Normals[normal_index+4] = norm[1];
						Normals[normal_index+5] = norm[2];
						Normals[normal_index+6] = norm[0];
						Normals[normal_index+7] = norm[1];
						Normals[normal_index+8] = norm[2];
 
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
 
void Model_PLY::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);	
 	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3,GL_FLOAT,	0,Triangles);	
	glNormalPointer(GL_FLOAT, 0, Normals);
	glDrawArrays(GL_TRIANGLES, 0, 3*TotalFaces);	
	glDisableClientState(GL_VERTEX_ARRAY);    
	glDisableClientState(GL_NORMAL_ARRAY);
}
#endif
