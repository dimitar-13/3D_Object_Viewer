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
	/*For more info about the shader: https://www.shadertoy.com/view/l3yXRc */

	vec2 uv = FragUV * uvScale;

	vec3 FinalColor = vec3(0.0);

	uv = sin(uv*3.14);
    float v = uv.x*uv.y;
    v = clamp( .5+.5* v/fwidth(v) ,0.,1. );

	FragColor = vec4(mix( GrayCol, WhiteCol, v) , 1.0);
}


