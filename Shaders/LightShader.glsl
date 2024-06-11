#Shader:vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uvCoords;
layout(location = 2) in vec3 normals;

out vec3 FragPosition;
out vec3 FragNormal;

uniform mat4 ViewProjMatrix;
//TODO:Add model matrix
void main()
{
	gl_Position = ViewProjMatrix*vec4(position,1);

	FragNormal = normals;
	FragPosition = position.xyz;
}

#Shader:fragment
#version 330 core
#define MAX_LIGHTS 4
#define AMBIENT_AMOUNT .1

struct DirectionalLight{
	vec3 LightDirection;
	vec3 lightColor;
};

vec3 CalculateLight(vec3 fragNormal,vec3 fragPos,vec3 viewPosition,DirectionalLight light);


out vec4 FragColor;
in vec3 FragPosition;
in vec3 FragNormal;


uniform DirectionalLight lights[MAX_LIGHTS];
uniform vec3 cameraPosition;
void main()
{
	vec3 Color = vec3(0,0,0);
	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		Color += CalculateLight(FragNormal,FragPosition,cameraPosition,lights[i]);
	}
	FragColor = vec4(Color,1);
}
vec3 CalculateLight(vec3 fragNormal,vec3 fragPos,vec3 viewPosition,DirectionalLight light)
{
	vec3 ambient = light.lightColor * AMBIENT_AMOUNT;
	light.LightDirection = normalize(light.LightDirection);
	fragNormal = normalize(fragNormal);

	float diff = max(0.,dot(fragNormal,-light.LightDirection));
	vec3 diffuse = light.lightColor * diff;

	vec3 viewDir =normalize(viewPosition - fragPos);
	vec3 halfVector = normalize(viewDir + -light.LightDirection);

	float spec =pow(max(0.,dot(fragNormal,halfVector)),128.);
	vec3 specular = light.lightColor * spec* float(diff != 0);

	return ambient + diffuse +specular;
}


