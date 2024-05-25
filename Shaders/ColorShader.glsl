#Shader:vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uvCoords;

out vec2 FragCoords;
uniform mat4 ViewProjMatrix;
void main()
{
	gl_Position = ViewProjMatrix*vec4(position,1);
	FragCoords = uvCoords;
}
#Shader:fragment
#version 330 core
out vec4 FragColor;
in vec2 FragCoords;

uniform sampler2D albedoTexture;
void main()
{
	FragColor = vec4(texture(albedoTexture,FragCoords).xyz,1);
}


