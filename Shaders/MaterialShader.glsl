#Shader:vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uvCoords;
layout(location = 2) in vec3 normals;
layout(location = 3) in vec3 tangent;

out FRAG_INFO
{
	out vec2 FragUV;
	out vec3 FragPosition;
	out mat3 TBN_Mat;
	out vec3 normals;
}vertex_out;

layout(std140) uniform Matrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	mat4 ModelMatrix;
};
void main()
{
	gl_Position =  ProjectionMatrix*ViewMatrix*ModelMatrix*vec4(position,1);
	vertex_out.FragUV =uvCoords;
	vertex_out.FragPosition = (ModelMatrix* vec4(position,1.0)).xyz;
	vec3 W_normals = normalize(mat3(ModelMatrix) * normals);
	vec3 W_tangent = normalize(mat3(ModelMatrix) * tangent);
	vertex_out.normals = W_normals;
	vertex_out.TBN_Mat = mat3(W_tangent,normalize(cross(W_tangent,W_normals)),W_normals);
}

#Shader:fragment
#version 330 core

struct Material{
	vec3 color;
	sampler2D albedo;
};

out vec4 FragColor;

in FRAG_INFO{
	in vec2 FragUV;
	in vec3 FragPosition;
	in mat3 TBN_Mat;
	in vec3 normals;
}fragment_in;



uniform Material Mesh_material;

void main()
{
	vec3 Color = vec3(0,0,0);
	
	vec3 FragCol = texture(Mesh_material.albedo,fragment_in.FragUV).xyz*Mesh_material.color;

	FragColor = vec4(FragCol,1);
}


