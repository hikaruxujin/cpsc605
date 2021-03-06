
varying vec3 n;
varying vec3 v; 

#define MAX_LIGHTS 3 
#define PI 3.14159265
void main (void)  
{  
   vec4 Color = vec4(0.0, 0.0, 0.0, 0.0);
   for (int i=0;i<MAX_LIGHTS;i++)
   {
	   vec3 N = normalize(n);
	   vec3 L = normalize(gl_LightSource[i].position.xyz - v);   
	   vec3 V = normalize(-v); 
	   vec3 H = normalize(L+V);  
	   vec3 R = normalize(-reflect(L,N));
	   //calculate diffuse 
	   vec4 Idiff = gl_FrontLightProduct[i].diffuse * max(dot(N,L), 0.0);
	   Idiff = clamp(Idiff, 0.0, 1.0);     
	   float factor = (gl_FrontMaterial.shininess+2.0)/(8.0*PI);
	   // calculate specular 	   
	   vec4 Ispec = gl_FrontLightProduct[i].specular*pow(max(dot(R,V),0.0),gl_FrontMaterial.shininess)
	   * pow(max(dot(H,N),0.0),gl_FrontMaterial.shininess);
	   //~ vec4 Ispec = gl_FrontLightProduct[i].specular*pow(max(dot(R,V),0.0),gl_FrontMaterial.shininess)
	   //~ * factor;
	   Ispec = clamp(Ispec, 0.0, 1.0); 
 
	   Color += Idiff + Ispec;
	}     
	gl_FragColor = Color;  
}
