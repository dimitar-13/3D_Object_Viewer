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
out vec2 FragUV;
void main()
{
	gl_Position = ProjectionMatrix*ViewMatrix*ModelMatrix*vec4(position,1);
	FragUV = uvCoords;
}

#Shader:fragment
#version 330 core
#define SMOOTHNESS_FACTOR 22.f
out vec4 FragColor;

in vec2 FragUV;
const vec3 GrayCol = vec3(.45);
const vec3 WhiteCol = vec3(.92);
uniform float uvScale;

void main()
{
	vec2 uv = FragUV * uvScale;

	vec3 FinalColor = vec3(0.0);
	uv = mod(uv, vec2(2.0f));

	float isCubeWhiteColX = step(2,step(1,uv.x)+1 - step(1,uv.y));

	float isCubeWhiteColY = step(2,1 - step(1,uv.x) + step(1,uv.y));

	float isCubeWhiteCol = max(isCubeWhiteColX, isCubeWhiteColY);
	
	float mixFactor = isCubeWhiteCol*min(
	(1. - pow(abs(fract(uv.x)*2. -1.),SMOOTHNESS_FACTOR)),
	(1. - pow(abs(fract(uv.y)*2. -1.),SMOOTHNESS_FACTOR)));

	FinalColor = mix(WhiteCol,GrayCol,mixFactor);

	FragColor = vec4(FinalColor, 1.0);
}


