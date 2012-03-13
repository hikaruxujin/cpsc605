varying vec3 ec_normal,ec_position;
varying vec4 tcoords;

void main(void)
{
	ec_normal = gl_NormalMatrix*gl_Normal;
	ec_position = vec3(gl_ModelViewMatrix*gl_Vertex);
	gl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex;
	tcoords = gl_TextureMatrix[7]*gl_Vertex;
}
