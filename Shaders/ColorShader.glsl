#Shader:vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uvCoords;
layout(location = 2) in vec3 normals;
layout(location = 3) in vec3 tangent;

layout(std140) uniform Matrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	mat4 ModelMatrix;
};

void main()
{
	gl_Position =  ProjectionMatrix*ViewMatrix*ModelMatrix*vec4(position,1);
}

#Shader:fragment
#version 330 core

out vec4 FragColor;
uniform vec3 u_Color;

void main()
{
	FragColor = vec4(u_Color,1);
}


