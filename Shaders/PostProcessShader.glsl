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

#include "ColorSpaceUtilityFunctions.glsl"


#define EDGE_THRESHOLD_MIN 0.0312
#define EDGE_THRESHOLD_MAX 0.125
#define SUBPIXEL_QUALITY  0.75

#define ITTERATIONS 12

const vec3 luma  = vec3(0.299, 0.587, 0.114);
out vec4 FragColor;
in vec2 FragUV;

uniform sampler2D u_framebufferTexture;
uniform vec2 u_resolution;
uniform bool u_useAA = true;

float FromRGBToLuma(vec3 col) {
    return (dot(luma, col));
}
vec4 GetColorFXAA();

void main() {
    vec4 sampledPixel = texture(u_framebufferTexture, FragUV);
    
    if (!u_useAA) {
        FragColor = sampledPixel;       
    }
	else
	{
		FragColor = GetColorFXAA();
	}

	
	FragColor = fromLinear(FragColor);

}
vec4 GetColorFXAA()
{
    vec4 sampledPixel = texture(u_framebufferTexture, FragUV);
    
    vec2 sizeOfPixel = 1.0 / u_resolution;
    
    vec3 LeftPixelCol = textureOffset(u_framebufferTexture, FragUV, ivec2(-1, 0)).rgb;
    vec3 UpPixelCol = textureOffset(u_framebufferTexture, FragUV, ivec2(0, 1)).rgb;
    vec3 RightPixelCol = textureOffset(u_framebufferTexture, FragUV, ivec2(1, 0)).rgb;
    vec3 DownPixelCol = textureOffset(u_framebufferTexture, FragUV, ivec2(0, -1)).rgb;
    vec3 CenterPixelCol = sampledPixel.rgb;
    
    float LeftPixelLuma = FromRGBToLuma(LeftPixelCol);
    float UpPixelLuma = FromRGBToLuma(UpPixelCol);
    float RightPixelLuma = FromRGBToLuma(RightPixelCol);
    float DownPixelLuma = FromRGBToLuma(DownPixelCol);
    float CenterPixelLuma = FromRGBToLuma(CenterPixelCol);
    
    float minLuma = min(CenterPixelLuma, min(LeftPixelLuma, min(UpPixelLuma, min(RightPixelLuma, DownPixelLuma))));
    float maxLuma = max(CenterPixelLuma, max(LeftPixelLuma, max(UpPixelLuma, max(RightPixelLuma, DownPixelLuma))));
    
    float lumaRange = maxLuma - minLuma;
    if (lumaRange < max(EDGE_THRESHOLD_MIN, maxLuma * EDGE_THRESHOLD_MAX)) {
        return sampledPixel;
    }
    
    float TL_luma = FromRGBToLuma(textureOffset(u_framebufferTexture, FragUV, ivec2(-1, 1)).rgb);
    float DL_luma = FromRGBToLuma(textureOffset(u_framebufferTexture, FragUV, ivec2(-1, -1)).rgb);
    float TR_luma = FromRGBToLuma(textureOffset(u_framebufferTexture, FragUV, ivec2(1, 1)).rgb);
    float DR_luma = FromRGBToLuma(textureOffset(u_framebufferTexture, FragUV, ivec2(1, -1)).rgb);
    
    float edgeHorizontal = abs((TL_luma - LeftPixelLuma) - (LeftPixelLuma - DL_luma)) +
                           2.0 * abs((UpPixelLuma - CenterPixelLuma) - (CenterPixelLuma - DownPixelLuma)) +
                           abs((TR_luma - RightPixelLuma) - (RightPixelLuma - DR_luma));
    
    float edgeVertical = abs((TR_luma - UpPixelLuma) - (UpPixelLuma - TL_luma)) +
                         2.0 * abs((RightPixelLuma - CenterPixelLuma) - (CenterPixelLuma - LeftPixelLuma)) +
                         abs((DR_luma - DownPixelLuma) - (DownPixelLuma - DL_luma));
    
    bool isHorizontal = edgeHorizontal >= edgeVertical;
    
    float luma1 = isHorizontal ? DownPixelLuma : LeftPixelLuma;
    float luma2 = isHorizontal ? UpPixelLuma : RightPixelLuma;
    
    float gradient1 = luma1 - CenterPixelLuma;
    float gradient2 = luma2 - CenterPixelLuma;
    
    bool isGradient1Bigger = abs(gradient1) >= abs(gradient2);
    
    float gradientScaled = 0.25 * max(abs(gradient1), abs(gradient2));
    
    float singlePixelOffset = isHorizontal ? sizeOfPixel.y : sizeOfPixel.x;
    
    float lumaAvrg = isGradient1Bigger ? 0.5 * (CenterPixelLuma + luma1) : 0.5 * (CenterPixelLuma + luma2);
    
    singlePixelOffset = isGradient1Bigger ? -singlePixelOffset : singlePixelOffset;
    
    vec2 currentUV = FragUV;
    
    currentUV += vec2(float(!isHorizontal) * singlePixelOffset * 0.5, float(isHorizontal) * singlePixelOffset * 0.5);
    
    vec2 offset = isHorizontal ? vec2(sizeOfPixel.x, 0.0) : vec2(0.0, sizeOfPixel.y);
    
    // We move in both negative and positive direction
    vec2 uv1 = currentUV - offset;
    vec2 uv2 = currentUV + offset;
    
    float LumaEndLeft = FromRGBToLuma(texture(u_framebufferTexture, uv1).rgb);
    float LumaEndRight = FromRGBToLuma(texture(u_framebufferTexture, uv2).rgb);
    
    LumaEndLeft -= lumaAvrg;
    LumaEndRight -= lumaAvrg;
    
    bool RightEndPointReached = abs(LumaEndRight) >= gradientScaled;
    bool LeftEndPointReached = abs(LumaEndLeft) >= gradientScaled;
    
    uv1 -= !LeftEndPointReached ? offset : vec2(0);
    uv2 += !RightEndPointReached ? offset : vec2(0);
    
    if (!(RightEndPointReached && LeftEndPointReached)) {
        for (int i = 1; i < ITTERATIONS; i++) {
            LumaEndLeft = FromRGBToLuma(texture(u_framebufferTexture, uv1).rgb);
            LumaEndRight = FromRGBToLuma(texture(u_framebufferTexture, uv2).rgb);
    
            LumaEndLeft -= lumaAvrg;
            LumaEndRight -= lumaAvrg;
    
            RightEndPointReached = abs(LumaEndRight) >= gradientScaled;
            LeftEndPointReached = abs(LumaEndLeft) >= gradientScaled;
    
            if (RightEndPointReached && LeftEndPointReached) break;
    
           uv1 -= !LeftEndPointReached ? offset : vec2(0);
           uv2 += !RightEndPointReached ? offset : vec2(0);
        }
    }
    
    float distance1 = isHorizontal ? FragUV.x - uv1.x : FragUV.y - uv1.y;
    float distance2 = isHorizontal ? uv2.x - FragUV.x : uv2.y - FragUV.y;
    
    float closestDist = min(distance1, distance2);
    
    float edgeThickness = (distance1 + distance2);
    bool isDirection1 = distance1 < distance2;
    
    float pixelOffset = -closestDist / edgeThickness + 0.5;
    
    bool isLumaCenterSmaller = CenterPixelLuma < lumaAvrg;
    
    bool correctVariation = ((isDirection1 ? LumaEndLeft : LumaEndRight) < 0.0) != isLumaCenterSmaller;
    
    float finalOffset = correctVariation ? pixelOffset : 0.0;
    
    float lumaDownUp = DownPixelLuma + UpPixelLuma;
    float lumaLeftRight = LeftPixelLuma + RightPixelLuma;
    
    float lumaLeftCorners = DL_luma + TL_luma;
    float lumaRightCorners = DR_luma + TR_luma;
    
    float lumaAverage = (1.0 / 12.0) * (2.0 * (lumaDownUp + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);
    
    float subPixelOffset1 = clamp(abs(lumaAverage - CenterPixelLuma) / lumaRange, 0.0, 1.0);
    float subPixelOffset2 = (-2.0 * subPixelOffset1 + 3.0) * subPixelOffset1 * subPixelOffset1;
    
    float subPixelOffsetFinal = subPixelOffset2 * subPixelOffset2 * SUBPIXEL_QUALITY;
    
    finalOffset = max(finalOffset, subPixelOffsetFinal);
    
    vec2 finalUV = !isHorizontal ? vec2(FragUV.x + finalOffset * singlePixelOffset, FragUV.y) : vec2(FragUV.x, FragUV.y + finalOffset * singlePixelOffset);
    
	return texture(u_framebufferTexture, finalUV);
}