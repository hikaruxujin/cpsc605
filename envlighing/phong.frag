
varying vec3 ec_normal;
varying vec3 ec_reflect;
 
uniform sampler2D diffuse_irr_map;
uniform sampler2D specular_irr_map;

void main (void)  
{  
		vec2 d_irr_index,s_irr_index;
		vec3 N,R,d_irr,s_irr;
		vec4 diffuse_color = gl_FrontMaterial.diffuse;
		vec4 specular_color = gl_FrontMaterial.specular;
		
		N = 0.99*normalize(ec_normal);
		R = 0.99*normalize(ec_reflect);
		
		d_irr_index = 0.5*(N.st + vec2(1.0,1.0));
		s_irr_index = 0.5*(R.st + vec2(1.0,1.0));
		
		d_irr = vec3(texture2D(diffuse_irr_map,d_irr_index));
		s_irr = vec3(texture2D(specular_irr_map,s_irr_index));
		
		diffuse_color *= vec4(d_irr,1.0);
		specular_color *= vec4(s_irr,1.0);
		
		gl_FragColor = diffuse_color + 0.5*specular_color; 
		//~ gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}
