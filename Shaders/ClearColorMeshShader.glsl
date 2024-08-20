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
	mat4 NormalMatrix;
};
out vec3 FragNormal;
out vec3 FragPosition;

void main()
{
	gl_Position = ProjectionMatrix*ViewMatrix*ModelMatrix*vec4(position,1);
	FragPosition = vec3(ViewMatrix*ModelMatrix*vec4(position,1));
	FragNormal = mat3(ViewMatrix)*mat3(NormalMatrix) * normals;
	FragNormal = normalize(FragNormal);
}

#Shader:fragment
#version 330 core

#include "ShaderCommonFunctions.glsl"


out vec4 FragColor;
in vec3 FragNormal;
in vec3 FragPosition;

uniform vec3 u_Color;

void main()
{	
	FragColor = vec4(GetStudioShading(FragPosition,FragNormal,u_Color),1);
}
