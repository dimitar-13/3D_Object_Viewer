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

#Shader:geometry
#version 330 core
layout(triangles) in;
layout(triangle_strip,max_vertices = 3) out;

uniform mat3 viewportMatrix;
noperspective out vec3 distance;

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
    //Transform from homogenous to NDC and from NDC to viewport(only the x and y values)
    vec3 p1 = (viewportMatrix *  (gl_in[0].gl_Position.xyz/gl_in[0].gl_Position.w)).xyz;
    vec3 p2 = (viewportMatrix * (gl_in[1].gl_Position.xyz/gl_in[1].gl_Position.w)).xyz;
    vec3 p3 = (viewportMatrix * (gl_in[2].gl_Position.xyz/gl_in[2].gl_Position.w)).xyz;

    float a =length(p2 - p3);
    float b =length(p3 - p1);
    float c =length(p2 - p1);

    float alpha = acos( (b*b + c*c - a*a) / (2.0*b*c) );
    float beta = acos( (a*a + c*c - b*b) / (2.0*a*c) );

    float ha = abs( c * sin( beta ) );
    float hb = abs( c * sin( alpha ) );
    float hc = abs( b * sin( alpha ) );
        

    //On enim every out variable is outputted                                       
	gl_Position = gl_in[0].gl_Position; 
    distance = vec3( ha, 0, 0 );

    geo_out_studioData.FractLightSpacePos = geo_in_studioData[0].FractLightSpacePos;
    geo_out_studioData.FractLightSpaceNormal = geo_in_studioData[0].FractLightSpaceNormal;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    distance = vec3( 0, hb, 0 );
    geo_out_studioData.FractLightSpacePos = geo_in_studioData[1].FractLightSpacePos;
    geo_out_studioData.FractLightSpaceNormal = geo_in_studioData[1].FractLightSpaceNormal;

    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    distance = vec3( 0, 0, hc );
    geo_out_studioData.FractLightSpacePos = geo_in_studioData[2].FractLightSpacePos;
    geo_out_studioData.FractLightSpaceNormal = geo_in_studioData[2].FractLightSpaceNormal;
    EmitVertex();

    EndPrimitive();
}

#Shader:fragment
#version 330 core

#include "ShaderCommonFunctions.glsl"

out vec4 FragColor;
noperspective in vec3 distance;

in VS_OUT_STUDIO_DATA{
    vec3 FractLightSpacePos;
    vec3 FractLightSpaceNormal;
}fs_in_studioData;


uniform vec3 u_frameColor;
uniform float frameThickness;


void main()
{
    float minDist = min(distance.x,min(distance.y,distance.z));
    float mixVal = smoothstep(frameThickness - 1, frameThickness + 1, minDist );
    vec3 FinalColor = vec3(mix( u_frameColor,
    GetStudioShading(fs_in_studioData.FractLightSpacePos,fs_in_studioData.FractLightSpaceNormal,vec3(1)), mixVal ));
    FragColor = vec4(FinalColor,1);
}
