uniform float twist;

void main()
{
	float angle,c,s;
	mat2 rotation;
	vec2 new_position;
	
	angle = twist*length(gl_Vertex.xy);
	//printf("%d",twist);
	s = sin(angle);
	c = cos(angle);
	rotation = mat2(c,-s,s,c);
	new_position = rotation*gl_Vertex.xy;
	gl_Position = vec4(new_position,0.0,1.0);
	gl_FrontColor = gl_Color;
}
