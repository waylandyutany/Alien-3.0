#include "frame_consts.h"
#include "lights.h"

// *****************************************************************************************************************
// *****************************************************************************************************************
struct S_VertexInput
{
    float4 pos : POSITION;
};

struct S_PixelInput
{
    float4 pos : SV_POSITION;
};

// ********************************************************************************
// Shader constants
// ********************************************************************************

cbuffer PerObjectConstants : register(b1)
{
    matrix worldMatrix;
};

struct S_Instance
{
    matrix instanceMatrix;
};

StructuredBuffer<S_Instance> instancies : register(t1);

// ********************************************************************************
// Vertex shader
// ********************************************************************************

S_PixelInput MainVS(S_VertexInput input, uint iid : SV_InstanceID)
{
    S_PixelInput output;

    input.pos.w = 1.0f;

    output.pos = mul(input.pos, instancies[iid].instanceMatrix);
    output.pos = mul(output.pos, worldMatrix);

    return output;
}

// *****************************************************************************************************************
// *****************************************************************************************************************

[maxvertexcount(14)]
void MainGS( triangle S_PixelInput input[3], inout TriangleStream<S_PixelInput> OutputStream )
{	
	const float shadowExtrusion = 0.01f;
	const float shadowLength = 200.0f;

	S_PixelInput p[6];

	float3 a = input[1].pos - input[0].pos;
	float3 b = input[2].pos - input[0].pos;
	float3 normal = cross(a,b); // we dont have to normalize

	for(int i = 0; i < numDirLights; i++)
	{
		float3 direction = dirLights[i].direction.xyz;
		// if facing toward to light (>0) we skip shadow volume extrusion
		const float lightFacing = dot(normal, -direction);
		if(lightFacing > 0) continue;
		
		// near shadow triangle points
		p[0].pos.xyz = input[0].pos.xyz + direction*shadowExtrusion;
		p[1].pos.xyz = input[1].pos.xyz + direction*shadowExtrusion;
		p[2].pos.xyz = input[2].pos.xyz + direction*shadowExtrusion;

		// far shadow triangle points
		p[3].pos.xyz = input[0].pos.xyz + direction*shadowLength;
		p[4].pos.xyz = input[1].pos.xyz + direction*shadowLength;
		p[5].pos.xyz = input[2].pos.xyz + direction*shadowLength;

		for(int j = 0; j < 6; j++)
		{
			p[j].pos.w = 1.0f;
			p[j].pos = mul(p[j].pos, projViewMatrix3D);
		}

		// shadow volume creation

		// near cap
		OutputStream.Append(p[2]);
		OutputStream.Append(p[1]);
		OutputStream.Append(p[0]);
		OutputStream.RestartStrip();

		// far cap
		OutputStream.Append(p[3]);
		OutputStream.Append(p[4]);
		OutputStream.Append(p[5]);
		OutputStream.RestartStrip();

		// silhuetes
		OutputStream.Append(p[3]);
		OutputStream.Append(p[0]);
		OutputStream.Append(p[4]);
		OutputStream.Append(p[1]);
		OutputStream.Append(p[5]);
		OutputStream.Append(p[2]);
		OutputStream.Append(p[3]);
		OutputStream.Append(p[0]);
		OutputStream.RestartStrip();
	}
}

// *****************************************************************************************************************
// *****************************************************************************************************************

float4 MainPS(S_PixelInput input) : SV_TARGET
{
	const float4 color = {1,1,1,1};
    return color;
}
