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
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 viewportMatrix;

noperspective out vec4 Edge_A_position_and_direction;
noperspective out vec4 Edge_B_position_and_direction;
noperspective out vec2 fragmentPos;
noperspective out vec3 distances;

flat out int projection_case;

struct PointIndicesTable {
    int A_table_index;
    int B_table_index;
    int A_prime_table_index;
    int B_prime_table_index;
};

const PointIndicesTable projection_case_indices_array[8] = PointIndicesTable[](
    PointIndicesTable(1, 1, 1, 1),  
    PointIndicesTable(0, 1, 2, 2),  
    PointIndicesTable(0, 2, 1, 1),  
    PointIndicesTable(0, 0, 1, 2),  
    PointIndicesTable(1, 2, 0, 0),  
    PointIndicesTable(1, 1, 0, 2),  
    PointIndicesTable(2, 2, 0, 1),  
    PointIndicesTable(0, 0, 0, 0)   
);

vec3 triangle_viewport_space_points[3];

in VS_OUT_STUDIO_DATA {
    vec3 FractLightSpacePos;
    vec3 FractLightSpaceNormal;
} geo_in_studioData[];

out VS_OUT_STUDIO_DATA {
    vec3 FractLightSpacePos;
    vec3 FractLightSpaceNormal;
} geo_out_studioData;

void main() {

    int final_case_value = 0;

    for (int i = 0; i < 3; i++) {
        triangle_viewport_space_points[i] = (viewportMatrix * (gl_in[i].gl_Position / gl_in[i].gl_Position.w)).xyz;
        int bit_value = (gl_in[i].gl_Position.z > 0) ? 0 : (1 << 2 - i);
        final_case_value += bit_value;
    }


    float a = length(triangle_viewport_space_points[1].xy - triangle_viewport_space_points[2].xy);
    float b = length(triangle_viewport_space_points[2].xy - triangle_viewport_space_points[0].xy);
    float c = length(triangle_viewport_space_points[1].xy - triangle_viewport_space_points[0].xy);

    float alpha = acos((b * b + c * c - a * a) / (2.0 * b * c));
    float beta = acos((a * a + c * c - b * b) / (2.0 * a * c));

    float ha = abs(c * sin(beta));
    float hb = abs(c * sin(alpha));
    float hc = abs(b * sin(alpha));

    vec3 edge_computed_distances = vec3(ha, hb, hc);

    vec3 A_viewport_space = triangle_viewport_space_points[projection_case_indices_array[final_case_value].A_table_index];
    vec3 B_viewport_space = triangle_viewport_space_points[projection_case_indices_array[final_case_value].B_table_index];
	vec3 A_prime_viewport_space = triangle_viewport_space_points[projection_case_indices_array[final_case_value].A_prime_table_index];
    vec3 B_prime_viewport_space = triangle_viewport_space_points[projection_case_indices_array[final_case_value].B_prime_table_index];

    vec2 triangle_edge_A_dir_viewport = normalize(A_viewport_space.xy - A_prime_viewport_space.xy);
    vec2 triangle_edge_B_dir_viewport = normalize(B_viewport_space.xy - B_prime_viewport_space.xy);

    for (int i = 0; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;

        projection_case = final_case_value;

        vec3 current_vertex_dist_vector = vec3(0);
        current_vertex_dist_vector[i] = edge_computed_distances[i];

        distances = current_vertex_dist_vector;

        Edge_A_position_and_direction.zw = triangle_edge_A_dir_viewport;
        Edge_A_position_and_direction.xy = A_viewport_space.xy;

        Edge_B_position_and_direction.zw = triangle_edge_B_dir_viewport;
        Edge_B_position_and_direction.xy = B_viewport_space.xy;

        fragmentPos = triangle_viewport_space_points[i].xy;

        geo_out_studioData.FractLightSpacePos = geo_in_studioData[i].FractLightSpacePos;
        geo_out_studioData.FractLightSpaceNormal = geo_in_studioData[i].FractLightSpaceNormal;
        EmitVertex();
    }
}


#Shader:fragment
#version 330 core

#include "ShaderCommonFunctions.glsl"

out vec4 FragColor;

noperspective in vec4 Edge_A_position_and_direction;
noperspective in vec4 Edge_B_position_and_direction;

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
	/**
	 * This shader is based on Nvidia's solid wireframe shader. See more here:
	 * Whitepaper: https://developer.download.nvidia.com/SDK/10/direct3d/Source/SolidWireframe/Doc/SolidWireframe.pdf
	 * Nvidia SDK (where you can find their source code. Fair warning: source code is in DirectX): https://developer.download.nvidia.com/SDK/10/direct3d/samples.html#SolidWireframe
	 * Below is my explanation of how the tricky case works. If you don’t understand the standard case (case 0), check this YouTube video;
	 * it helped me a lot: https://www.youtube.com/watch?v=P6DnNoS46Fs
	 */

	/**
	 * A little context:
	 *
	 * First, if you don’t want me to spoil the fun of understanding the tricky case, then don’t read this.
	 *
	 * At first glance, I wasn’t sure why we did this: 
	 * minDist = abs(A_to_fragment_squared - A_to_fragment_cos * A_to_fragment_cos);
	 * And then I took a look at it. This is the Pythagorean theorem.
	 *
	 * The tricky case logic tries to find a distance from a fragment to an edge. To achieve this, we have a direction of the edge
	 * and a point (A) on that edge.
	 *	           ^(A)
	 *	 edge a-- /|\
	 *			 / | \
	 *		 P<-/__|  \
	 *		   /   F   \
	 *
     * If you look at the triangle above, you will see we want to find the length of FP (for the limited way of drawing, assume 
     * FP is perpendicular to edge A). Now because FP is perpendicular, 
	 * that means AF is the hypotenuse of the triangle, AP is the adjacent, and FP is the opposite.
     *
     * This leaves us with hyp^2 = adjacent^2 + opposite^2, or in our case AF^2 = AP^2 + FP^2. Rearranging the expression, we get:
     *     FP^2 = AF^2 - AP^2.
     *
     * - AF is easy; it is just the vector difference of point F and A (the vector from A to F).
     * Now by having the direction of the edge, we can get the cosine between AF and AP. Why would we want to do it?
     * Well, from the right triangle trigonometric rule, we can say cos = adjacent/hyp, rearranging we get cos * hyp = adjacent.
     * Replacing it in Pythagoras, we get FP^2 = AF^2 - (cos * hyp)^2.
     * To get the dot product, we do dot(AF, direction of edge A). Now because AF is a non-unit vector, we actually get cos * (magnitude of AF),
     * and if you recall, AF is the hypotenuse. That’s why we got 'A_to_fragment_cos * A_to_fragment_cos', which is actually (cos * (magnitude of AF))^2.
     * At the end of the algorithm, we do the sqrt to get the distance. 	 
     */

	float minDist;
	vec3 FinalColor;

	if(projection_case == 0)
	{
		minDist = min(distances.x,min(distances.y,distances.z));
	}
	else if(projection_case == 7)
	{
		FinalColor = GetStudioShading(fs_in_studioData.FractLightSpacePos,fs_in_studioData.FractLightSpaceNormal,vec3(1));
		return;
	}
	else
	{
		vec2 A_to_fragment_vector = fragmentPos - Edge_A_position_and_direction.xy;
		vec2 B_to_fragment_vector = fragmentPos - Edge_B_position_and_direction.xy;
		float A_to_fragment_cos =  dot(Edge_A_position_and_direction.zw,A_to_fragment_vector);
		float B_to_fragment_cos =  dot(Edge_B_position_and_direction.zw,B_to_fragment_vector);

		float A_to_fragment_squared = dot(A_to_fragment_vector ,A_to_fragment_vector );
		float B_to_fragment_squared = dot(B_to_fragment_vector ,B_to_fragment_vector );


		minDist = abs(A_to_fragment_squared - A_to_fragment_cos*A_to_fragment_cos);
		minDist = min(minDist,abs(B_to_fragment_squared - B_to_fragment_cos*B_to_fragment_cos));
		if(projection_case== 1 || projection_case == 2 || projection_case == 4)
		{
			float AFcosA0 = dot(A_to_fragment_vector,normalize(Edge_A_position_and_direction.xy - Edge_B_position_and_direction.xy));

			minDist = min(minDist,abs(A_to_fragment_squared -AFcosA0*AFcosA0 ));
		}
		minDist = sqrt(minDist);
	}


    float mixVal = smoothstep(frameThickness - 1, frameThickness + 1, minDist );
    FinalColor = vec3(mix( u_frameColor,vec3(1), mixVal));
	FinalColor = GetStudioShading(fs_in_studioData.FractLightSpacePos,fs_in_studioData.FractLightSpaceNormal,FinalColor);

    FragColor = vec4(FinalColor,1);
	

#ifdef WIREFRAME_PROJECTION_CASE_DEBUG_OUTPUT

	 if (projection_case == 0) {
        FragColor = vec4(1, 1, 1, 1);
   } if (projection_case == 1) {
        FragColor = vec4(1, 0, 0, 1);
    } else if (projection_case == 2) {
        FragColor = vec4(0, 1, 0, 1); 
    } else if (projection_case == 3) {
        FragColor = vec4(0, 0, 1, 1);
    } else if (projection_case == 4) {
        FragColor = vec4(1, 1, 0, 1);
    } else if (projection_case == 5) {
        FragColor = vec4(1, 0, 1, 1); 
    } else if (projection_case == 6) {
        FragColor = vec4(0, 1, 1, 1); 
    } else if (projection_case == 7) {
        FragColor = vec4(0.5, 0.5, 0.5, 1);
    }
#endif		
}
