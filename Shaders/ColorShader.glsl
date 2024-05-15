#Shader:vertex
#version 330 core
layout(location = 0) in vec2 position;

uniform mat4 ViewProjMatrix;
void main()
{
	gl_Position = ViewProjMatrix*vec4(position,0,1);
}
#Shader:fragment
#version 330 core
out vec4 FragColor;
void main()
{
	FragColor = vec4(1,1,1,1);
}


