#Shader:vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uvCoords;
uniform mat4 ViewProjMatrix;
void main()
{
	gl_Position = ViewProjMatrix*vec4(position,1);
}
#Shader:fragment
#version 330 core
out vec4 FragColor;
void main()
{
	FragColor = vec4(1,1,1,1);
}


