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
void main()
{
	gl_Position =  ProjectionMatrix*ViewMatrix*ModelMatrix*vec4(position,1);
}

#Shader:fragment
#version 330 core

out vec4 FragColor;
noperspective in vec3 distance;

uniform vec3 u_frameColor;
uniform float frameThickness;
void main()
{
    float minDist = min(distance.x,min(distance.y,distance.z));
    float mixVal = smoothstep(frameThickness - 1.,frameThickness + 1.,minDist);
	FragColor = vec4(vec3(mix(u_frameColor,vec3(1),mixVal)),1);
}

#Shader:geometry
#version 330 core
layout(triangles) in;
layout(triangle_strip,max_vertices = 3) out;

uniform mat3 viewportMatrix;
noperspective out vec3 distance;
void main()
{
    //Transform from homogenous to NDC and from NDC to viewport(only the x and y values)
    vec3 p1 = (viewportMatrix *  (gl_in[0].gl_Position.xyz/gl_in[0].gl_Position.w)).xyz;
    vec3 p2 = (viewportMatrix * (gl_in[1].gl_Position.xyz/gl_in[1].gl_Position.w)).xyz;
    vec3 p3 = (viewportMatrix * (gl_in[2].gl_Position.xyz/gl_in[2].gl_Position.w)).xyz;

    float a =length(p3 - p1);
    float b =length(p1 - p2);
    float c =length(p3 - p2);

    float alpha = acos( (b*b + c*c - a*a) / (2.0*b*c) );
    float beta = acos( (a*a + c*c - b*b) / (2.0*a*c) );

    float ha = abs( c * sin( beta ) );
    float hb = abs( c * sin( alpha ) );
    float hc = abs( b * sin( alpha ) );
        

    //On enim every out variable is outputted                                       
	gl_Position = gl_in[0].gl_Position; 
    distance = vec3( ha, 0, 0 );
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    distance = vec3( 0, hb, 0 );
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    distance = vec3( 0, 0, hc );
    EmitVertex();

    EndPrimitive();
}
