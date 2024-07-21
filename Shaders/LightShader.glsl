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
#define MAX_LIGHTS 4
#define AMBIENT_AMOUNT .1

struct DirectionalLight{
	vec3 LightDirection;
	vec3 lightColor;
};

struct Material{
	vec3 color;
	float specular;
	sampler2D albedo;
	sampler2D roughtness;
	sampler2D normalMap;
	sampler2D ambientOcclusion;
};
struct FragLightData
{
	vec3 FragColor;
	vec3 FragPosition;
	vec3 FragSpecularVal;
	vec3 FragNormal;
	float materialShininess;
};
vec3 CalculateLight(FragLightData fragData,vec3 viewPosition,DirectionalLight light);


out vec4 FragColor;

in FRAG_INFO{
	in vec2 FragUV;
	in vec3 FragPosition;
	in mat3 TBN_Mat;
	in vec3 normals;
}fragment_in;


layout(std140) uniform LightInfo
{
	DirectionalLight lights[MAX_LIGHTS];
};

uniform Material Mesh_material;
uniform vec3 cameraPosition;
uniform bool isToonShadingOn;
uniform bool isRimLightOn;


const int toonColorLevel = 4;
const float recipricalToonLevel = 1./toonColorLevel;
const float rimLightPowerFactor = 6;
float GetRimLightFactor(vec3 CameraToPixel,vec3 fragNormal);
void main()
{
	vec3 Color = vec3(0,0,0);
	
	vec3 SampledNormals = texture(Mesh_material.normalMap,fragment_in.FragUV).xyz*2. - 1.;
	
	vec3 FragNormal = fragment_in.TBN_Mat*SampledNormals;
	
	vec3 FragCol = texture(Mesh_material.albedo,fragment_in.FragUV).xyz*Mesh_material.color;

	FragCol *= texture(Mesh_material.ambientOcclusion,fragment_in.FragUV).xyz;

	FragLightData data;

	data.FragColor = FragCol;
	data.FragPosition = fragment_in.FragPosition;
	data.FragSpecularVal = texture(Mesh_material.roughtness,fragment_in.FragUV).xyz;
	data.FragNormal = FragNormal;
	data.materialShininess = Mesh_material.specular;

	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		Color += CalculateLight(data,cameraPosition,lights[i]);
	}
	FragColor = vec4(Color,1);
}
vec3 CalculateLight(FragLightData fragData,vec3 viewPosition,DirectionalLight light)
{
	vec3 ambient = light.lightColor  * fragData.FragColor * AMBIENT_AMOUNT;
	light.LightDirection = normalize(light.LightDirection);

	vec3 fragNormal = normalize(fragData.FragNormal);

	float diff = max(0.,dot(fragNormal,-light.LightDirection));

	if(isToonShadingOn)
		diff = ceil(diff*toonColorLevel)*recipricalToonLevel;

	vec3 diffuseColor = light.lightColor* fragData.FragColor;
	vec3 diffuse = diffuseColor * diff;
	
	vec3 viewDir =normalize(viewPosition - fragData.FragPosition);

	vec3 RimColor = vec3(0,0,0);
	if(isRimLightOn)
		RimColor = diffuseColor*GetRimLightFactor(viewDir,fragNormal);

	vec3 halfVector = normalize(viewDir + -light.LightDirection);
	float spec = pow(max(0.,dot(fragNormal,halfVector)),fragData.materialShininess);
	vec3 specular = light.lightColor * spec* float(diff != 0) * fragData.FragSpecularVal;

	return ambient + diffuse + RimColor + specular*(isToonShadingOn || isRimLightOn  ? 0 : 1);
}

float GetRimLightFactor(vec3 CameraToPixel,vec3 fragNormal)
{
	CameraToPixel = normalize(CameraToPixel);
	fragNormal = normalize(fragNormal);

	return pow(max(1 - dot(CameraToPixel,fragNormal),0),rimLightPowerFactor);

}


