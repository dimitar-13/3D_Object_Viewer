
vec3 GetStudioShading(vec3 postitionInViewSpace,vec3 normalInViewSpace,vec3 shadingColor)
{
	const float LIGHT_POW_FACTOR = 2.f; 
	normalInViewSpace = normalize(abs(normalInViewSpace));

	vec3 fragToCamDir = normalize(-postitionInViewSpace);
	float lightFactor = pow(max(dot(fragToCamDir,normalInViewSpace),0),LIGHT_POW_FACTOR);

	return shadingColor*lightFactor;
}
