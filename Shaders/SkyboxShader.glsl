#Shader:vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uvCoords;

out vec3 FragCoords;
layout(std140) uniform Matrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	mat4 ModelMatrix;
};
void main()
{
	vec4 pos = ProjectionMatrix*ViewMatrix*vec4(position,1);
	gl_Position = pos.xyww;
	FragCoords = position;
}
#Shader:fragment
#version 330 core
out vec4 FragColor;
in vec3 FragCoords;

uniform samplerCube  skyboxSampler;
void main()
{
	FragColor = texture(skyboxSampler,FragCoords);
}
