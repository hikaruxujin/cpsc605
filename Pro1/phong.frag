
varying vec3 n;
varying vec3 v; 

#define MAX_LIGHTS 3 
   
void main (void)  
{  
   vec4 Color = vec4(0.0, 0.0, 0.0, 0.0);
   for (int i=0;i<MAX_LIGHTS;i++)
   {
	   vec3 N = normalize(n);
	   vec3 L = normalize(gl_LightSource[i].position.xyz - v);   
	   vec3 V = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	   vec3 R = normalize(-reflect(L,N));  


	   //calculate Diffuse Term:  
	   vec4 Idiff = gl_FrontLightProduct[i].diffuse * max(dot(N,L), 0.0);
	   Idiff = clamp(Idiff, 0.0, 1.0);     
	   
	   // calculate Specular Term:
	   vec4 Ispec = gl_FrontLightProduct[i].specular 
					* pow(max(dot(R,V),0.0),gl_FrontMaterial.shininess);
	   Ispec = clamp(Ispec, 0.0, 1.0); 
	   // write Total Color:  
	   Color += Idiff + Ispec;
	}     
	gl_FragColor = Color;  
}
