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

out VS_OUT_STUDIO_DATA{
     vec3 FractLightSpacePos;
     vec3 FractLightSpaceNormal;
}vs_out_studioData;

void main()
{
    vec4 vertexLightSpace = ViewMatrix*ModelMatrix*vec4(position,1);
	gl_Position = ProjectionMatrix*vertexLightSpace;
    vs_out_studioData.FractLightSpacePos = vertexLightSpace.xyz;

   	vs_out_studioData.FractLightSpaceNormal = mat3(ViewMatrix)*mat3(NormalMatrix) * normals;
	vs_out_studioData.FractLightSpaceNormal = normalize(vs_out_studioData.FractLightSpaceNormal);
}

#Shader:fragment
#version 330 core

out vec4 FragColor;
uniform vec3 u_Color;
uniform float pointSize;
noperspective in vec3 pointDist;

in VS_OUT_STUDIO_DATA{
    vec3 FractLightSpacePos;
    vec3 FractLightSpaceNormal;
}fs_in_studioData;

flat in vec3 vertex0;
flat in vec3 vertex1;
flat in vec3 vertex2;
const float smoothness = 1.;

vec3 GetStudioLightShading();

void main()
{
    float maxFactor = min(smoothstep(pointSize - smoothness, pointSize + smoothness, length(pointDist - vertex0)),
    min(smoothstep(pointSize - smoothness, pointSize + smoothness, length(pointDist - vertex1)),
    smoothstep(pointSize - smoothness, pointSize + smoothness, length(pointDist - vertex2))));

    vec3 FinalColor = vec3(mix( u_Color, GetStudioLightShading() , maxFactor ));
	FragColor = vec4(FinalColor,1);
}
vec3 GetStudioLightShading()
{
    const float LIGHT_POW_FACTOR = 2.f; 

	vec3 fragToCamDir = normalize(-fs_in_studioData.FractLightSpacePos);
	float lightFactor = pow(max(dot(fragToCamDir,fs_in_studioData.FractLightSpaceNormal),0),LIGHT_POW_FACTOR);

    return vec3(vec3(1)*lightFactor);
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

in VS_OUT_STUDIO_DATA{
     vec3 FractLightSpacePos;
     vec3 FractLightSpaceNormal;
}geo_in_studioData[];

out VS_OUT_STUDIO_DATA{
     vec3 FractLightSpacePos;
     vec3 FractLightSpaceNormal;
}geo_out_studioData;

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
    geo_out_studioData.FractLightSpacePos = geo_in_studioData[0].FractLightSpacePos;
    geo_out_studioData.FractLightSpaceNormal = geo_in_studioData[0].FractLightSpaceNormal;

    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    pointDist = vec3(p2);
    vertex0 = p1;
    vertex1 = p2;
    vertex2 = p3;
       geo_out_studioData.FractLightSpacePos = geo_in_studioData[1].FractLightSpacePos;
    geo_out_studioData.FractLightSpaceNormal = geo_in_studioData[1].FractLightSpaceNormal;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    pointDist = vec3(p3);
    vertex0 = p1;
    vertex1 = p2;
    vertex2 = p3;
       geo_out_studioData.FractLightSpacePos = geo_in_studioData[2].FractLightSpacePos;
    geo_out_studioData.FractLightSpaceNormal = geo_in_studioData[2].FractLightSpaceNormal;
    EmitVertex();

    EndPrimitive();
}
