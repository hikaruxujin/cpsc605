void main()
{
    vec3 my_displaced_vertex;
    my_displaced_vertex = 0.25*gl_Vertex.xyz + vec3(0.5,0.5,0.0);
    gl_Position = vec4(my_displaced_vertex,1.0);
    gl_FrontColor = vec4(0.0,1.0,1.0,1.0);
}
