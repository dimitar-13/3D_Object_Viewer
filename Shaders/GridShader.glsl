#Shader:vertex
#version 330 core
layout(location = 0) in vec3 position;

out vec3 farPoint;
out vec3 nearPoint;

out mat4 frag_viewMatrix;
out mat4 frag_projMatrix;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

vec3 unprojectPoint(vec4 point,mat4 viewMatrix,mat4 ProjectionMatrix)
{
	mat4 inverseView = inverse(viewMatrix);
	mat4 inversePorj = inverse(ProjectionMatrix);
	point = inverseView *inversePorj *point;
	return point.xyz/point.w;
}

void main()
{
	frag_viewMatrix = ViewMatrix;
	frag_projMatrix = ProjectionMatrix;

	farPoint = unprojectPoint(vec4(position.xy,1.0,1.0),ViewMatrix,ProjectionMatrix).xyz;
	nearPoint = unprojectPoint(vec4(position.xy,0.0,1.0),ViewMatrix,ProjectionMatrix).xyz;

	gl_Position = vec4(position,1);
}
#Shader:fragment
#version 330 core

out vec4 FragColor;
in vec3 farPoint;
in vec3 nearPoint;
in mat4 frag_viewMatrix;
in mat4 frag_projMatrix;

float near = 0.1; 
float far  = 100.0; 
uniform vec3 cameraPosition;
struct GridInfo
{
	float gridScale;
	vec3 gridLineColor;
	bool isAxisShaded;
};
uniform GridInfo gridInfo;

float GetFragDepth(vec3 fragPos)
{
	vec4 pos = frag_projMatrix * frag_viewMatrix * vec4(fragPos,1.0);
	return pos.z/pos.w;
}

vec4 GetGridCol(vec2 p,GridInfo grid_Info)
{
	vec2 coords = p*grid_Info.gridScale;
	vec2 subregions = abs(fract(coords -.5)-.5);

	vec3 gridColor = vec3(grid_Info.gridLineColor);

    vec2 dCoords = fwidth(coords);
	// Dynamically adjust smoothstep based on derivatives
	float dX = smoothstep(0.0, dCoords.x * 2.0, subregions.x);
	float dY = smoothstep(0.0, dCoords.y * 2.0, subregions.y);

	float d = 1.0 - (dX * dY);
	
	if(step(.01,coords.x) - step(.1,coords.x) >0. && grid_Info.isAxisShaded)
	{
		gridColor = vec3(0,0,.5);
		d = 1.;
	}
		
	if(step(.01,coords.y) - step(.1,coords.y) > 0. && grid_Info.isAxisShaded)
	{
		gridColor = vec3(.5,.0,0);
		d = 1.;
	}
	return vec4(gridColor,d);
}

void main()
{
	float t = -nearPoint.y / (farPoint.y - nearPoint.y);
   
	vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);
	gl_FragDepth = GetFragDepth(fragPos3D);

	vec4 col = GetGridCol(fragPos3D.xz,gridInfo);
	
	col.a *= 1- smoothstep(length(cameraPosition.xz ),length(cameraPosition.xz )+3., length(fragPos3D.xz));
	col.a *= t < 0 ? 0. :1.;
	FragColor = vec4(col);	
}


