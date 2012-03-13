varying vec3 ec_normal;
varying vec3 ec_reflect;

void main(void)  
{
	vec4 ec_vertex;
	vec3 ec_eyedir;     
  
   ec_normal = normalize(gl_NormalMatrix * gl_Normal);
   ec_vertex = gl_ModelViewMatrix*gl_Vertex;
   ec_eyedir = normalize(-ec_vertex.xyz);
   ec_reflect = -ec_eyedir + 2.0*(dot(ec_eyedir,ec_normal))*ec_normal;
   gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex; 
}
