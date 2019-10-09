#include "screen_quad_gs.h"
#include "samplers.h"

// *****************************************************************************************************************
// *****************************************************************************************************************

Texture2D diffuseTexture	: register(t0);
Texture2D lightTexture		: register(t1);

float4 MainPS(S_PixelInput input) : SV_TARGET
{
	float4 diffuse = diffuseTexture.Sample(samplerPointWrap, input.tex);
	float4 light = lightTexture.Sample(samplerPointWrap, input.tex);

	// Apply ambient color and ambient intensity
	float4 color = ambientColor;

	// Apply lights
	color += light;
	color = saturate(color);

	// Apply texture
	color *= diffuse;

	// Apply specularity
	color = saturate(color + (light.w * 0.2f));

	// Apply global color and global intensity
	color *= globalColor;

	// Set alpha to 1
	color.w = 1.0f;

    return color;
}
