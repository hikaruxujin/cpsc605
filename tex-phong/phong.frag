
varying vec3 N;
varying vec3 v;    
uniform sampler2D mytexture;
void main (void)  
{  
   vec3 L = normalize(gl_LightSource[0].position.xyz - v);   
   vec3 V = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)  
   vec3 H = normalize(L+V);  
  

   //calculate Diffuse Term:
   vec3 tcolor = vec3(texture2D(mytexture,gl_TexCoord[0].st));
     
   vec4 Idiff = 0.5 * gl_FrontMaterial.diffuse + 0.5 * vec4(tcolor,1.0);
   Idiff *= max(dot(N,L),0.0);
   Idiff = clamp(Idiff, 0.0, 1.0);     
   
   // calculate Specular Term:
   vec4 Ispec = gl_FrontMaterial.specular * pow(max(dot(H,N),0.0),gl_FrontMaterial.shininess);
   Ispec = clamp(Ispec, 0.0, 1.0); 
   // write Total Color:  
   gl_FragColor = Idiff + Ispec;     
}
