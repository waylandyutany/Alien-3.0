#include "frame_consts.h"

// ********************************************************************************
// Shaders inputs/outputs
// ********************************************************************************
struct S_VertexInput
{
    float4 pos : POSITION;
    uint   iid : SV_InstanceID;
};

struct S_PixelInput
{
    float4 pos   : SV_POSITION;
    float4 color : COLOR0;
    uint   iid   : SV_InstanceID;
};

// ********************************************************************************
// Vertex shader
// ********************************************************************************

S_PixelInput MainVS(S_VertexInput input)
{
    S_PixelInput output;

    output.pos = input.pos;
    output.iid = input.iid;

    return output;
}

// ********************************************************************************
// Geometry shader
// ********************************************************************************
struct S_3DElement
{
	float3 p0,p1,p2;
	float4 color;
	uint   flags;
};

StructuredBuffer<S_3DElement> elements : register(t0);

void TransformPoint(inout float4 p, float3 pos)
{
	p.w = 1.0f;
	p.xyz = pos.xyz;
	p = mul(p, projViewMatrix3D);
}

[maxvertexcount(6)]
void MainGS( triangle S_PixelInput input[3], inout TriangleStream<S_PixelInput> OutputStream )
{	
    S_PixelInput p[3];

	uint elementIndex = input[0].iid;
	uint elementFlags = elements[elementIndex].flags;
	uint elementType = elementFlags & 0xf;
	bool isSolid = elementFlags & (1 << 4);

	if(!isSolid) return;

	p[0].iid = elementFlags;
	p[1].iid = elementFlags;
	p[2].iid = elementFlags;

	TransformPoint(p[0].pos,elements[elementIndex].p0);
	p[0].color = elements[elementIndex].color;

	TransformPoint(p[1].pos,elements[elementIndex].p1);
	p[1].color = elements[elementIndex].color;

	TransformPoint(p[2].pos,elements[elementIndex].p2);
	p[2].color = elements[elementIndex].color;

	// Front face
	OutputStream.Append(p[0]);
	OutputStream.Append(p[1]);
	OutputStream.Append(p[2]);
	OutputStream.RestartStrip();
}

// ********************************************************************************
// Pixel shader
// ********************************************************************************

float4 MainPS(S_PixelInput input) : SV_TARGET
{
    return input.color;
}
