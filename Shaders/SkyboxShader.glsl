#Shader:vertex
#version 330 core
layout(location = 0) in vec3 position;

out vec3 FragCoords;
layout(std140) uniform Matrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	mat4 ModelMatrix;
	mat4 NormalMatrix;
};
uniform bool uIsProjectionPerspective;
void main()
{
	mat4 skyboxView = mat4(mat3(ViewMatrix));
	mat4 finalProjection = uIsProjectionPerspective ? ProjectionMatrix : mat4(1);

	vec4 pos = finalProjection*skyboxView*vec4(position,1);
	gl_Position =uIsProjectionPerspective ? pos.xyww : pos.xyzz;
	FragCoords = position;
}
#Shader:fragment
#version 330 core

#include "ColorSpaceUtilityFunctions.glsl"

out vec4 FragColor;
in vec3 FragCoords;

uniform samplerCube skyboxSampler;
void main()
{

	FragColor = vec4(toLinear(texture(skyboxSampler,FragCoords)).rgb,1);
}
