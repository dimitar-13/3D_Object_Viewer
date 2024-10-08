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
out vec2 FragUV;

out VS_OUT_STUDIO_DATA{
     vec3 FractLightSpacePos;
     vec3 FractLightSpaceNormal;
}vs_out_studioData;

void main()
{
	vec4 vertexViewSpace = ViewMatrix*ModelMatrix*vec4(position,1);
	gl_Position = ProjectionMatrix*vertexViewSpace;
	FragUV = uvCoords;

	vs_out_studioData.FractLightSpacePos = vertexViewSpace.xyz;
	vs_out_studioData.FractLightSpaceNormal = mat3(ViewMatrix)*mat3(NormalMatrix) * normals;
	vs_out_studioData.FractLightSpaceNormal = normalize(vs_out_studioData.FractLightSpaceNormal);
}

#Shader:fragment
#version 330 core

#include "ShaderCommonFunctions.glsl"

#define SMOOTHNESS_FACTOR 22.f
out vec4 FragColor;

in vec2 FragUV;
const vec3 GrayCol = vec3(.412);
const vec3 WhiteCol = vec3(1);
uniform float uvScale;

in VS_OUT_STUDIO_DATA{
    vec3 FractLightSpacePos;
    vec3 FractLightSpaceNormal;
}fs_in_studioData;

vec3 GetStudioLightShading(vec3 color);

void main()
{
	/*For more info about the shader: https://www.shadertoy.com/view/l3yXRc */

	vec2 uv = FragUV * uvScale;

	vec3 FinalColor = vec3(0.0);

	uv = sin(uv*3.14);
    float v = uv.x*uv.y;
    v = clamp( .5+.5* v/fwidth(v) ,0.,1. );

	vec3 AACheckerboardCol = mix( GrayCol, WhiteCol, v);
	
	vec3 ApplyStudioLightShading =
	GetStudioShading(fs_in_studioData.FractLightSpacePos,fs_in_studioData.FractLightSpaceNormal,AACheckerboardCol);

	FragColor = vec4(ApplyStudioLightShading, 1.0);
}



