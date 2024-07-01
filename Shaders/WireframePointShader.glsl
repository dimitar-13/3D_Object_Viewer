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

void main()
{
    float d = max(pointDist.x,max(pointDist.y,pointDist.z));
	FragColor = vec4(vec3(smoothstep(0.0,.002,pointSize - d)),1);
}


#Shader:geometry
#version 330 core
layout(triangles) in;
layout(triangle_strip,max_vertices = 3) out;

uniform mat3 viewportMatrix;
noperspective out vec3 pointDist;

void main()
{
    //Transform from homogenous to NDC and from NDC to viewport(only the x and y values)
    vec2 p1 = (viewportMatrix *  (gl_in[0].gl_Position.xyz/gl_in[0].gl_Position.w)).xy;
    vec2 p2 = (viewportMatrix * (gl_in[1].gl_Position.xyz/gl_in[1].gl_Position.w)).xy;
    vec2 p3 = (viewportMatrix * (gl_in[2].gl_Position.xyz/gl_in[2].gl_Position.w)).xy;

    float a = length(p2 - p3);
    float b = length(p3 - p1);
    float c = length(p2 - p1);
        
    //On enim every out variable is outputted                                       
	gl_Position = gl_in[0].gl_Position; 
    pointDist = vec3( 1,0 ,0);
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    pointDist = vec3(  0,1, 0);
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    pointDist = vec3( 0,0 , 1 );
    EmitVertex();

    EndPrimitive();
}
