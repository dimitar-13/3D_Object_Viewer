#Shader:vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uvCoords;
layout(location = 2) in vec3 normals;

out vec2 FragCoords;
uniform mat4 ViewProjMatrix;
void main()
{
	gl_Position = ViewProjMatrix*vec4(position,1);
	FragCoords = uvCoords;
}
#Shader:fragment
#version 330 core
struct Material
{
	vec3 color;
	float roughness;
	sampler2D albedoTexture;
	sampler2D roughnessTexture;
	sampler2D normalTexture;
	sampler2D ambientOcclusion;
};
out vec4 FragColor;
in vec2 FragCoords;

uniform Material material;
void main()
{
	vec3 Color = length(texture(material.albedoTexture,FragCoords).xyz) != 0 ? texture(material.albedoTexture,FragCoords).xyz : material.color;
	FragColor = vec4(Color,1);
}


