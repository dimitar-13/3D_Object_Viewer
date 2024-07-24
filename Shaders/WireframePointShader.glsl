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
uniform mat3 viewportMatrix;
void main()
{
	gl_Position = ProjectionMatrix*ViewMatrix*ModelMatrix*vec4(position,1);
}

#Shader:fragment
#version 330 core

out vec4 FragColor;
uniform vec3 u_Color;
uniform float pointSize;
noperspective in vec3 pointDist;
flat in vec3 vertex0;
flat in vec3 vertex1;
flat in vec3 vertex2;
const float smoothness = 1.;
void main()
{
    float maxFactor = min(smoothstep(pointSize - smoothness, pointSize + smoothness, length(pointDist - vertex0)),
    min(smoothstep(pointSize - smoothness, pointSize + smoothness, length(pointDist - vertex1)),
    smoothstep(pointSize - smoothness, pointSize + smoothness, length(pointDist - vertex2))));

    vec3 FinalColor = vec3(mix( u_Color, vec3(1), maxFactor ));
	FragColor = vec4(FinalColor,1);
}


#Shader:geometry
#version 330 core
layout(triangles) in;
layout(triangle_strip,max_vertices = 3) out;
uniform mat3 viewportMatrix;
noperspective out vec3 pointDist;
flat out vec3 vertex0;
flat out vec3 vertex1;
flat out vec3 vertex2;
void main()
{     

    vec3 p1 = (viewportMatrix * (gl_in[0].gl_Position.xyz/gl_in[0].gl_Position.w)).xyz;
    vec3 p2 = (viewportMatrix * (gl_in[1].gl_Position.xyz/gl_in[1].gl_Position.w)).xyz;
    vec3 p3 = (viewportMatrix * (gl_in[2].gl_Position.xyz/gl_in[2].gl_Position.w)).xyz;

	gl_Position = gl_in[0].gl_Position;
    pointDist = vec3(p1);
    vertex0 = p1;
    vertex1 = p2;
    vertex2 = p3;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    pointDist = vec3(p2);
    vertex0 = p1;
    vertex1 = p2;
    vertex2 = p3;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    pointDist = vec3(p3);
    vertex0 = p1;
    vertex1 = p2;
    vertex2 = p3;
    EmitVertex();

    EndPrimitive();
}
