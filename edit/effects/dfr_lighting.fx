#include "screen_quad_gs.h"
#include "samplers.h"
#include "lights.h"

// *****************************************************************************************************************
// *****************************************************************************************************************

Texture2D normalTexture			: register(t0);
Texture2D<float> depthTexture	: register(t1);
Texture2D<float> shadowDepthTexture	: register(t2);

// *****************************************************************************************************************
// todo use of half lambert factor, pow(((lightIntensity * 0.5f) + 0.5f),2);, intensity goes from (-1,1) -> (0,1)
float4 MainPS(S_PixelInput input) : SV_TARGET
{
	float4 light = {0,0,0,0};
	float4 normal = normalTexture.Sample(samplerPointWrap, input.tex);
	float depth = depthTexture.Sample(samplerPointWrap, input.tex);
// *****************************************************************************************************************
	float4 screenSpace = {(input.tex.x * 2.0f) - 1.0f, 1.0f - (input.tex.y * 2.0f), depth, 1.0f};
	float4 worldPos = mul(screenSpace, invProjViewMatrix3D);
	worldPos /= worldPos.w;
// *****************************************************************************************************************
	const float3 viewDirection = normalize(viewPos - worldPos);
// *****************************************************************************************************************
	[fastopt]
	for(uint i = 0; i < numDirLights; i++)
	{
		// shadow map stuff
		// correct !!!
		float4 shadowMapTex = mul(worldPos, dirLights[i].worldToLightTex);
		shadowMapTex /= shadowMapTex.w;

		float shadowDepth = 1.0f;
		float4 shadowMapPoint = {0,0,0,0};
		if(shadowMapTex.x == saturate(shadowMapTex.x) && shadowMapTex.y == saturate(shadowMapTex.y))
		{
			shadowDepth = shadowDepthTexture.Sample(samplerPointClamp, shadowMapTex.xy);
			shadowMapPoint = mul(worldPos, dirLights[i].worldToLightProj);
			shadowMapPoint.z /= shadowMapPoint.w;
		}

//		shadowMapPoint.z -= 0.0025f;
		if(shadowMapPoint.z < shadowDepth) // we are not in shadow
		{
			// light stuff
			const float lightIntensity = saturate(dot(normal.xyz, -dirLights[i].direction.xyz));
			if(lightIntensity > 0)
			{
				light.xyz += lightIntensity * dirLights[i].color.xyz;
				const float reflection = normalize(2*lightIntensity*normal - dirLights[i].direction.xyz);
				light.w = max(light.w, saturate(dot(reflection,viewDirection)));
			}
		}
	}
// *****************************************************************************************************************
	[fastopt]
	for(uint i = 0; i < numPointLights; i++)
	{
		float3 lightDirection = worldPos.xyz - pointLights[i].posAndRadius.xyz;
		float distanceToLight = length(lightDirection);
		lightDirection = normalize(lightDirection);

		if(distanceToLight < pointLights[i].posAndRadius.w)
		{
			const float lightAttenuation = 1.0f - (distanceToLight / pointLights[i].posAndRadius.w);
			if(lightAttenuation > 0)
			{
				const float lightIntensity = saturate(dot(normal.xyz, -lightDirection));
				if(lightIntensity > 0)
				{
					light.xyz += lightIntensity * lightAttenuation * pointLights[i].color.xyz;

					const float reflection = normalize(2*lightAttenuation*normal - lightDirection);
					light.w = max(light.w, saturate(dot(reflection,viewDirection)) * lightAttenuation);
				}
			}
		}
	}
// *****************************************************************************************************************
    return light;
}
