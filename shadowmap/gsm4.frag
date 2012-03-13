uniform sampler2D mytexture;
varying vec3 ec_normal, ec_position;
varying vec4 tcoords;

void main() {
	vec3 P, N, L, V, H;
	vec4 pccoords;
	vec4 diffuse_color = gl_FrontMaterial.diffuse;
	vec4 specular_color = gl_FrontMaterial.specular;
	float shininess = gl_FrontMaterial.shininess;
	float depthsample, clarity;
	
	P = ec_position;
	N = normalize(ec_normal);
	L = normalize(vec3(gl_LightSource[0].position) - P);
	V = normalize(-P);
	H = normalize(L+V);
	pccoords = tcoords/tcoords.w;
	depthsample = texture2D(mytexture, pccoords.st).z;
	clarity = 1.0;
	if (depthsample < pccoords.z) clarity = 0.5;
	diffuse_color *= clarity*max(dot(N,L), 0.0);
	specular_color *= clarity*pow(max(dot(H,N), 0.0), shininess);
	gl_FragColor = 0.5*(diffuse_color + specular_color);
	
	//~ gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}
