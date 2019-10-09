#include "frame_consts.h"

// *****************************************************************************************************************
// *****************************************************************************************************************

struct S_VertexInput
{
    float4 pos : POSITION;
    float2 tex : TEXCOORD0;
};

struct S_PixelInput
{
    float4 pos   : SV_POSITION;
    float2 tex   : TEXCOORD0;
};

// *****************************************************************************************************************
// *****************************************************************************************************************

S_PixelInput MainVS(S_VertexInput input, uint iid : SV_InstanceID)
{
    S_PixelInput output;

    output.pos = input.pos;
    output.tex = input.tex;

    return output;
}

// *****************************************************************************************************************
// *****************************************************************************************************************
[maxvertexcount(6)]
void MainGS( triangle S_PixelInput input[3], inout TriangleStream<S_PixelInput> OutputStream )
{	
	S_PixelInput p[4];

	// left bottom point
	p[0].pos.x = screenZeroPoint.x;
	p[0].pos.y = -screenZeroPoint.y;
	p[0].tex.x = 0.0f;
	p[0].tex.y = 1.0f;

	// left top point
	p[1].pos.x = screenZeroPoint.x;
	p[1].pos.y = screenZeroPoint.y;
	p[1].tex.x = 0.0f;
	p[1].tex.y = 0.0f;

	// right bottom point
	p[2].pos.x = -screenZeroPoint.x;
	p[2].pos.y = -screenZeroPoint.y;
	p[2].tex.x = 1.0f;
	p[2].tex.y = 1.0f;

	// right top point
	p[3].pos.x = -screenZeroPoint.x;
	p[3].pos.y = screenZeroPoint.y;
	p[3].tex.x = 1.0f;
	p[3].tex.y = 0.0f;

	for(int i = 0; i < 4; i++) {
		p[i].pos.z = 0.1f;
		p[i].pos.w = 1.0f;
		p[i].pos = mul(p[i].pos, projMatrix2D);
		OutputStream.Append(p[i]);
	}

	OutputStream.RestartStrip();
}
