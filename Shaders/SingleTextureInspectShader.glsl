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
out vec2 Frag_UV_Coords;

void main()
{
	gl_Position = ProjectionMatrix*ViewMatrix*ModelMatrix*vec4(position,1);
	Frag_UV_Coords = uvCoords;
}

#Shader:fragment
#version 330 core

#include "ColorSpaceUtilityFunctions.glsl"

out vec4 FragColor;
in vec2 Frag_UV_Coords;
uniform sampler2D textureToInspect;
uniform bool uIsTextureInSRGB;

void main()
{
	vec4 sampledTexel = texture(textureToInspect,Frag_UV_Coords);
	FragColor = uIsTextureInSRGB ? vec4(toLinear(sampledTexel)) : sampledTexel;
}


