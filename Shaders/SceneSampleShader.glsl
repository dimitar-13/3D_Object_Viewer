#Shader:vertex
#version 330 core
layout(location = 0) in vec3 position;
out vec2 FragUV;
void main()
{
	gl_Position = vec4(position,1);

	//Bring from range [-1:1] to [0:1]
	FragUV = (position.xy + 1.)/2.;
}

#Shader:fragment
#version 330 core

#define EDGE_THRESHOLD_MIN 0.0312
#define EDGE_THRESHOLD_MAX 0.125
#define ITTERATIONS 12

out vec4 FragColor;
in vec2 FragUV;

uniform sampler2D u_framebufferTexture;
uniform vec2 u_resolution;

void main()
{
	
	vec2 sizeOfPixel = 1/u_resolution;

	const vec3 lumaVector =  vec3(0.299,0.587, 0.114);

	vec4 sampledPixel =texture(u_framebufferTexture,FragUV); 

	vec3 LeftPixelCol = textureOffset(u_framebufferTexture,FragUV,ivec2(-1,0)).rgb;
	vec3 UpPixelCol = textureOffset(u_framebufferTexture,FragUV,ivec2(0,1)).rgb;
	vec3 RigthPixelCol = textureOffset(u_framebufferTexture,FragUV,ivec2(1,0)).rgb;
	vec3 DownPixelCol = textureOffset(u_framebufferTexture,FragUV,ivec2(0,-1)).rgb;
	vec3 CenterPixelCol = sampledPixel.rgb;

	float LeftPixelLuma = dot(LeftPixelCol,lumaVector);
	float UpPixelLuma = dot(UpPixelCol,lumaVector);
	float RigthPixelLuma = dot(RigthPixelCol,lumaVector);
	float DownPixelLuma = dot(DownPixelCol,lumaVector);
	float CenterPixelLuma = dot(CenterPixelCol,lumaVector);

	float minLuma = min(CenterPixelLuma,min(LeftPixelLuma,min(UpPixelLuma,min(RigthPixelLuma,DownPixelLuma))));
	float maxLuma = max(CenterPixelLuma,max(LeftPixelLuma,max(UpPixelLuma,max(RigthPixelLuma,DownPixelLuma))));

	float lumaRange = maxLuma - minLuma;
	if(lumaRange  < max(EDGE_THRESHOLD_MIN,maxLuma*EDGE_THRESHOLD_MAX))
	{
		FragColor = sampledPixel;
		return;
	}

	float TL_luma = dot(lumaVector,textureOffset(u_framebufferTexture,FragUV,ivec2(-1,1)).rgb);
	float DL_luma = dot(lumaVector,textureOffset(u_framebufferTexture,FragUV,ivec2(-1,-1)).rgb);
	float TR_luma = dot(lumaVector,textureOffset(u_framebufferTexture,FragUV,ivec2(1,1)).rgb);
	float DR_luma = dot(lumaVector,textureOffset(u_framebufferTexture,FragUV,ivec2(1,-1)).rgb);

	float edgeHorizontal = abs((TL_luma - LeftPixelLuma) - (LeftPixelLuma - DL_luma)) +
					2 * abs((UpPixelLuma - CenterPixelLuma) - (CenterPixelLuma - DownPixelLuma))+
					    abs((TR_luma - RigthPixelLuma) - (RigthPixelLuma - DR_luma));

	float edgeVerticle = abs((TR_luma - UpPixelLuma) - (UpPixelLuma - TL_luma))+ 
				 2 * abs((RigthPixelLuma - CenterPixelLuma) - (CenterPixelLuma - LeftPixelLuma))+
					 abs((DR_luma - DownPixelLuma) - (DownPixelLuma - DL_luma));

	bool isHorizontal = edgeHorizontal >= edgeVerticle;

	float luma1 = isHorizontal ? DownPixelLuma : LeftPixelLuma;
	float luma2 = isHorizontal ? UpPixelLuma : RigthPixelLuma;

	float gradient1 = luma1 - CenterPixelLuma;
	float gradient2 = luma2 - CenterPixelLuma;

	bool isGradient1Bigger = abs(gradient1) >= abs(gradient2);

	float gradientScaled = .25*max(abs(gradient1),abs(gradient2));

	float singlePixelOffset = isHorizontal ? sizeOfPixel.y : sizeOfPixel.x;

	float lumaAvrg = isGradient1Bigger ? .5*(CenterPixelLuma + luma1) : .5*(CenterPixelLuma + luma2);

	singlePixelOffset = isGradient1Bigger ? -singlePixelOffset : singlePixelOffset;



	vec2 currentUV = FragUV;

	currentUV += vec2(float(!isHorizontal)*singlePixelOffset*.5,float(isHorizontal)*singlePixelOffset*.5);

	vec2 offset = isHorizontal ? vec2(sizeOfPixel.x,0.0) : vec2(0.0,sizeOfPixel.y);

	//We move in both negative and positive direction
	vec2 uv1 = currentUV - offset;
	vec2 uv2 = currentUV + offset;

	float LumaEndLeft = dot(lumaVector,texture(u_framebufferTexture,uv1).rgb);
	float LumaEndRigth = dot(lumaVector,texture(u_framebufferTexture,uv2).rgb);

	LumaEndLeft -= lumaAvrg;
	LumaEndRigth -= lumaAvrg;

	bool RightEndPointReached = abs(LumaEndRigth) >= gradientScaled;
	bool LeftEndPointReached = abs(LumaEndLeft) >= gradientScaled;

	uv1 -= float(!LeftEndPointReached)  * offset;
	uv2 += float(!RightEndPointReached) * offset;

	if(!(RightEndPointReached && LeftEndPointReached))
	{
		for(int i = 1; i < ITTERATIONS; i ++)
		{
			LumaEndLeft = dot(lumaVector,texture(u_framebufferTexture,uv1).rgb);
			LumaEndRigth = dot(lumaVector,texture(u_framebufferTexture,uv2).rgb);

			LumaEndLeft -= lumaAvrg;
			LumaEndRigth -= lumaAvrg;


			RightEndPointReached = abs(LumaEndRigth) >= gradientScaled;
			LeftEndPointReached = abs(LumaEndLeft) >= gradientScaled;

			if(RightEndPointReached && LeftEndPointReached) break;

			uv1 -= float(!LeftEndPointReached)  * offset;
			uv2 += float(!RightEndPointReached) * offset;	
		}
	}

	float distance1 = isHorizontal ? FragUV.x -  uv1.x :FragUV.y -  uv1.y;
	float distance2 = isHorizontal ?  uv2.x - FragUV.x: uv2.y - FragUV.y;

	float closesestDist = min(distance1, distance2);

	float edgeThickness = (distance1 + distance2);
	bool isDirection1 = distance1 < distance2;

	float pixelOffset = - closesestDist / edgeThickness + 0.5;

	bool isLumaCenterSmaller = CenterPixelLuma < lumaAvrg;
	

	bool correctVariation = ((isDirection1 ? LumaEndLeft : LumaEndRigth) < 0.0) != isLumaCenterSmaller;
	
	float finalOffset = correctVariation ? pixelOffset : 0.0;


	vec2 finalUV = vec2(FragUV.x + finalOffset*singlePixelOffset*float(!isHorizontal),
	FragUV.y + finalOffset*singlePixelOffset*float(isHorizontal));

	FragColor = texture(u_framebufferTexture,finalUV);


}


