#include "frame_consts.h"
#include "samplers.h"
#include "lights.h"

// ********************************************************************************
// Shaders inputs/outputs
// ********************************************************************************

struct S_VertexInput
{
    float4 pos : POSITION;
    float2 tex :  TEXCOORD0;
};

struct S_PixelInput
{
    float4 pos : SV_POSITION;
    float4 depthPos : TEXCOORD0;
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
    output.pos = mul(output.pos, dirLights[0].worldToLightProj);
	output.depthPos = output.pos;

    return output;
}

// ********************************************************************************
// Geometry shader
// ********************************************************************************
[maxvertexcount(3)]

void MainGS( triangle S_PixelInput input[3], inout TriangleStream<S_PixelInput> OutputStream )
{	
    for( uint i=0; i<3; i++ )
    {
		S_PixelInput output;
        output.pos = input[i].pos;
		output.depthPos = input[i].depthPos;

        OutputStream.Append( output );
    }
	
    OutputStream.RestartStrip();
}

// ********************************************************************************
// Pixel shader
// ********************************************************************************
float MainPS(S_PixelInput input) : SV_TARGET
{
	return input.depthPos.z / input.depthPos.w;
}
