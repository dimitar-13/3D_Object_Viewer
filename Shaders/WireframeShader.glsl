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

uniform mat4 viewportMatrix;

flat out vec2 OutADir;
flat out vec2 A_viewport_position;
flat out vec2 OutBDir;
flat out vec2 B_viewport_position;

noperspective out vec2 fragmentPos;
noperspective out vec3 distances;
flat out int projection_case;

struct PointIndecies
{
	int A_projection_index;
	int B_projection_index;
	int A_prime_projection_index;
	int B_prime_projection_index;
};

const PointIndecies projection_case_indices_array[8] = PointIndecies[](
    PointIndecies(1, 1, 1, 1),
    PointIndecies(0, 1, 2, 2),
    PointIndecies(0, 2, 1, 1),
    PointIndecies(0, 0, 1, 2),
    PointIndecies(1, 2, 0, 0),
    PointIndecies(1, 1, 0, 2),
    PointIndecies(2, 2, 0, 1),
    PointIndecies(0, 0, 0, 0)
);

vec4 triangle_projection_space_points[3];
vec3 triangle_viewport_space_points[3];


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
	
	vec4 A_projection;
	vec4 B_projection;
	vec4 A_prime_projection;
	vec4 B_prime_projection;

	int final_case_value = 0;

    //Transform from homogenous to NDC and from NDC to viewport(only the x and y values)
	for(int i =0; i < 3;i ++)
	{
	    triangle_projection_space_points[i] = gl_in[i].gl_Position;
	    triangle_viewport_space_points[i] = (viewportMatrix*(gl_in[i].gl_Position.xyzw/gl_in[i].gl_Position.w)).xyz;
	    int bit_value = (triangle_projection_space_points[i].z > 0) ? 0 : (1 << (i));
	    final_case_value += bit_value;
	}
 
    float a =length(triangle_viewport_space_points[1].xy - triangle_viewport_space_points[2].xy);
    float b =length(triangle_viewport_space_points[2].xy - triangle_viewport_space_points[0].xy);
    float c =length(triangle_viewport_space_points[1].xy - triangle_viewport_space_points[0].xy);

    float alpha = acos( (b*b + c*c - a*a) / (2.0*b*c) );
    float beta = acos( (a*a + c*c - b*b) / (2.0*a*c) );

    float ha = abs( c * sin( beta ) );
    float hb = abs( c * sin( alpha ) );
    float hc = abs( b * sin( alpha ) );

	vec3 edge_computed_distances = vec3(ha,hb,hc);


	A_projection       = triangle_projection_space_points[projection_case_indices_array[final_case_value].A_projection_index];
	B_projection       = triangle_projection_space_points[projection_case_indices_array[final_case_value].B_projection_index];
	A_prime_projection = triangle_projection_space_points[projection_case_indices_array[final_case_value].A_prime_projection_index];
	B_prime_projection = triangle_projection_space_points[projection_case_indices_array[final_case_value].B_prime_projection_index];


	vec3 A_viewport_space = triangle_viewport_space_points[projection_case_indices_array[final_case_value].A_projection_index];
	vec3 B_viewport_space = triangle_viewport_space_points[projection_case_indices_array[final_case_value].B_projection_index];

	vec4 A_projection_calculations_proj_space = viewportMatrix*(A_prime_projection + A_projection)/2.f;
	vec4 B_projection_calculations_proj_space = viewportMatrix*(B_prime_projection + B_projection)/2.f;

	A_projection_calculations_proj_space/=  A_projection_calculations_proj_space.w;
	B_projection_calculations_proj_space/=  B_projection_calculations_proj_space.w;

    vec2 triangle_edge_A_dir_viewport = normalize(A_viewport_space - (A_projection_calculations_proj_space).xyz).xy;
    vec2 triangle_edge_B_dir_viewport = normalize(B_viewport_space - (B_projection_calculations_proj_space).xyz).xy;

       
    
	for(int i =0;i < 3 ;i ++)
	{
	    gl_Position = gl_in[i].gl_Position; 
	    
		projection_case = final_case_value;

		vec3 current_vertex_dist_vector = vec3(0);
		current_vertex_dist_vector[i] = edge_computed_distances[i];

		distances = current_vertex_dist_vector;

	    OutADir = triangle_edge_A_dir_viewport;
	    A_viewport_position = A_viewport_space.xy;
	    
	    OutBDir = triangle_edge_B_dir_viewport;
	    B_viewport_position = B_viewport_space.xy;
	    
	    fragmentPos = triangle_viewport_space_points[i].xy;
	    
        geo_out_studioData.FractLightSpacePos = geo_in_studioData[i].FractLightSpacePos;
        geo_out_studioData.FractLightSpaceNormal = geo_in_studioData[i].FractLightSpaceNormal;
        EmitVertex();
	}

}

#Shader:fragment
#version 330 core

#include "ShaderCommonFunctions.glsl"

#define WIREFRAME_PROJECTION_CASE_DEBUG_OUTPUTT
out vec4 FragColor;

flat in vec2 OutADir;
flat in vec2 A_viewport_position;
flat in vec2 OutBDir;
flat in vec2 B_viewport_position;

noperspective in vec2 fragmentPos;
noperspective in vec3 distances;
flat in int projection_case;

in VS_OUT_STUDIO_DATA{
    vec3 FractLightSpacePos;
    vec3 FractLightSpaceNormal;
}fs_in_studioData;


uniform vec3 u_frameColor;
uniform float frameThickness;


void main()
{
	vec2 fragment_to_edge_distances;

	vec2 A_to_fragment_vector = fragmentPos - A_viewport_position;
	vec2 B_to_fragment_vector = fragmentPos - B_viewport_position;

	fragment_to_edge_distances.x = sqrt(dot(A_to_fragment_vector ,A_to_fragment_vector ) - dot(OutADir,A_to_fragment_vector));
	fragment_to_edge_distances.y = sqrt(dot(B_to_fragment_vector ,B_to_fragment_vector ) - dot(OutBDir,B_to_fragment_vector));
	
    float minDist =projection_case == 0 ? min(distances.x,min(distances.y,distances.z)) :
	min(fragment_to_edge_distances.x,fragment_to_edge_distances.y);

    float mixVal = smoothstep(frameThickness - 1, frameThickness + 1, minDist );
    vec3 FinalColor = vec3(mix( u_frameColor,vec3(1), mixVal));
	FinalColor = GetStudioShading(fs_in_studioData.FractLightSpacePos,fs_in_studioData.FractLightSpaceNormal,FinalColor);

    FragColor = vec4(FinalColor,1);
	

#ifdef WIREFRAME_PROJECTION_CASE_DEBUG_OUTPUT

if (projection_case == 0) {
        FragColor = vec4(1, 1, 1, 1);
		}
    if (projection_case == 1) {
        FragColor = vec4(1, 0, 0, 1); // Red
    } else if (projection_case == 2) {
        FragColor = vec4(0, 1, 0, 1); // Green
    } else if (projection_case == 3) {
        FragColor = vec4(0, 0, 1, 1); // Blue
    } else if (projection_case == 4) {
        FragColor = vec4(1, 1, 0, 1); // Yellow
    } else if (projection_case == 5) {
        FragColor = vec4(1, 0, 1, 1); // Magenta
    } else if (projection_case == 6) {
        FragColor = vec4(0, 1, 1, 1); // Cyan
    } else if (projection_case == 7) {
        FragColor = vec4(0.5, 0.5, 0.5, 1); // Gray
    }
#endif		
}
