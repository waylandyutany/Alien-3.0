#include "frame_consts.h"
#include "samplers.h"

// *****************************************************************************************************************
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
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float4 depthPos : TEXCOORD1;
};

struct S_PixelOutput  
{  
  float4 diffuse_rt0 : COLOR0;  
  float4 normal_rt1	 : COLOR1;  
  float depth_rt2	 : COLOR2;  
};  

// ********************************************************************************
// Shader constants
// ********************************************************************************

cbuffer PerObjectConstants : register(b1)
{
    matrix worldMatrix;
};

cbuffer PerMaterialConstants : register(b2)
{
    float4 matColor;
	uint matFlags;
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
    output.normal = output.pos;
    output.pos = mul(output.pos, projViewMatrix3D);
	output.depthPos = output.pos;
    output.tex = input.tex;

    return output;
}

// ********************************************************************************
// Geometry shader
// ********************************************************************************
[maxvertexcount(3)]

void MainGS( triangle S_PixelInput input[3], inout TriangleStream<S_PixelInput> OutputStream )
{	
    S_PixelInput output;

	float3 a = input[1].normal - input[0].normal;
	float3 b = input[2].normal - input[0].normal;
	float3 n = normalize(cross(a,b));

    for( uint i=0; i<3; i++ )
    {
        output.pos = input[i].pos;
        output.tex = input[i].tex;
		output.depthPos = input[i].depthPos;
		output.normal = n;

        OutputStream.Append( output );
    }
	
    OutputStream.RestartStrip();
}

// ********************************************************************************
// Pixel shader
// ********************************************************************************
Texture2D diffuseTexture : register(t0);

S_PixelOutput MainPS(S_PixelInput input) : SV_TARGET
{
	S_PixelOutput psOut;

	float4 color = matColor;
	if(matFlags) color = diffuseTexture.Sample(samplerLinearMirror, input.tex);

	psOut.diffuse_rt0 = color;
	psOut.normal_rt1.xyz = input.normal.xyz;
	psOut.normal_rt1.w = 0;
	psOut.depth_rt2 = input.depthPos.z / input.depthPos.w;
    return psOut;
}
