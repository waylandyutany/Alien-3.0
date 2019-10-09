
// *****************************************************************************************************************
// *****************************************************************************************************************

cbuffer CB_Ligths : register(b3)
{
	uint numDirLights;
	uint numPointLights;
	uint padding0, padding1;
};

// *****************************************************************************************************************
// *****************************************************************************************************************
struct S_DirLight
{
	float4 color;		// Directional light color
	float4 direction;	// Directional light normalized direction
	matrix worldToLightProj;
	matrix worldToLightTex;
};

struct S_PointLight
{
	float4 color;			// Point light color
	float4 posAndRadius;	// xyz - point light world position, w - point light radius
};

StructuredBuffer<S_DirLight> dirLights : register(t4);
StructuredBuffer<S_PointLight> pointLights : register(t5);

// *****************************************************************************************************************
// *****************************************************************************************************************
