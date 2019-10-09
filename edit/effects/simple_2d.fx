#include "frame_consts.h"
#include "samplers.h"

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
    float4 color : COLOR0;
    uint   iid   : SV_InstanceID;
};

// *****************************************************************************************************************
// *****************************************************************************************************************

cbuffer PerMaterialConstants : register(b2)
{
    float4 matColor;
};

// *****************************************************************************************************************
// *****************************************************************************************************************

S_PixelInput MainVS(S_VertexInput input, uint iid : SV_InstanceID)
{
    S_PixelInput output;

    output.pos = input.pos;
    output.tex = input.tex;
    output.iid = iid;

    return output;
}

// *****************************************************************************************************************
// *****************************************************************************************************************

struct S_2DElement
{
	float2 pxy,psxy;
	float2 txy,tsxy;
	float4 color;
	uint   flags;
	float4 reserved;
};

StructuredBuffer<S_2DElement> elements : register(t0);

// *****************************************************************************************************************
// *****************************************************************************************************************
void TransformPoint(inout S_PixelInput p)
{
	p.pos.x += screenZeroPoint.x;
	p.pos.y = screenZeroPoint.y - p.pos.y;
	p.pos.x *= screenScale.x;
	p.pos.y *= screenScale.y;
}

void ProjectPoint(inout S_PixelInput p)
{
	p.pos.z = 0.1f;
	p.pos.w = 1.0f;
	p.pos = mul(p.pos, projMatrix2D);
}

void TransformAndProjectPoints4(inout S_PixelInput p[4])
{
	TransformPoint(p[0]);
	TransformPoint(p[1]);
	TransformPoint(p[2]);
	TransformPoint(p[3]);

	ProjectPoint(p[0]);
	ProjectPoint(p[1]);
	ProjectPoint(p[2]);
	ProjectPoint(p[3]);
}

// *****************************************************************************************************************
// *****************************************************************************************************************

void MakeBox(float2 pxy, float2 psxy, float4 color, inout S_PixelInput p[4])
{
	// left bottom point
	p[0].pos.x = pxy.x;
	p[0].pos.y = pxy.y + psxy.y;
	p[0].color = color;

	// left top point
	p[1].pos.x = pxy.x;
	p[1].pos.y = pxy.y;
	p[1].color = color;

	// right bottom point
	p[2].pos.x = pxy.x + psxy.x;
	p[2].pos.y = pxy.y + psxy.y;
	p[2].color = color;

	// right top point
	p[3].pos.x = pxy.x + psxy.x;
	p[3].pos.y = pxy.y;
	p[3].color = color;
}

void MakeBoxOutput(uint ei, inout S_PixelInput p[4])
{
	MakeBox(elements[ei].pxy, elements[ei].psxy, elements[ei].color, p);

	// left bottom point
	p[0].tex.x = elements[ei].txy.x;
	p[0].tex.y = elements[ei].txy.y + elements[ei].tsxy.y;

	// left top
	p[1].tex.x = elements[ei].txy.x;
	p[1].tex.y = elements[ei].txy.y;

	// right bottom point
	p[2].tex.x = elements[ei].txy.x + elements[ei].tsxy.x;
	p[2].tex.y = elements[ei].txy.y + elements[ei].tsxy.y;

	// right top point
	p[3].tex.x = elements[ei].txy.x + elements[ei].tsxy.x;
	p[3].tex.y = elements[ei].txy.y;

	TransformAndProjectPoints4(p);
}

// *****************************************************************************************************************
// *****************************************************************************************************************

void GetRectangleCorners(uint ei, inout float2 lb, inout float2 lt, inout float2 rb, inout float2 rt)
{
	// left bottom
	lb.x = elements[ei].pxy.x;
	lb.y = elements[ei].pxy.y + elements[ei].psxy.y;
	// left top
	lt = elements[ei].pxy;
	// right bottom
	rb.x = elements[ei].pxy.x + elements[ei].psxy.x;
	rb.y = elements[ei].pxy.y + elements[ei].psxy.y;
	// right top
	rt.x = elements[ei].pxy.x + elements[ei].psxy.x;
	rt.y = elements[ei].pxy.y;
}

void MakeSideLineBox( float2 p0, float rightx, float bottomy, float4 color, inout S_PixelInput p[4])
{
	MakeBox(p0, float2(rightx - p0.x,bottomy - p0.y), color, p);

	p[0].tex.x = 0;
	p[0].tex.y = 0;
	p[1].tex.x = 0;
	p[1].tex.y = 0;
	p[2].tex.x = 0;
	p[2].tex.y = 0;
	p[3].tex.x = 0;
	p[3].tex.y = 0;

	TransformAndProjectPoints4(p);
}

void GetStringArray( uint elementIndex, inout uint strArray[16])
{
	uint u32;
	u32 = asuint(elements[elementIndex].txy.x);
	strArray[0] = u32 & 0xff;
	strArray[1] = (u32 >> 8) & 0xff;
	strArray[2] = (u32 >> 16) & 0xff;
	strArray[3] = (u32 >> 24) & 0xff;

	u32 = asuint(elements[elementIndex].txy.y);
	strArray[4] = u32 & 0xff;
	strArray[5] = (u32 >> 8) & 0xff;
	strArray[6] = (u32 >> 16) & 0xff;
	strArray[7] = (u32 >> 24) & 0xff;

	u32 = asuint(elements[elementIndex].tsxy.x);
	strArray[8] = u32 & 0xff;
	strArray[9] = (u32 >> 8) & 0xff;
	strArray[10] = (u32 >> 16) & 0xff;
	strArray[11] = (u32 >> 24) & 0xff;

	u32 = asuint(elements[elementIndex].tsxy.y);
	strArray[12] = u32 & 0xff;
	strArray[13] = (u32 >> 8) & 0xff;
	strArray[14] = (u32 >> 16) & 0xff;
	strArray[15] = (u32 >> 24) & 0xff;
}

// *****************************************************************************************************************
// *****************************************************************************************************************


void MakeCharBox( float2 pt, float2 sz, uint chr, float4 color, inout S_PixelInput p[4])
{
	float chrStep = 0.0625f;
	uint chrx = chr % 16; 
	uint chry = chr / 16; 
	float2 t = { chrx * chrStep, chry * chrStep };


	MakeBox(pt, sz, color, p);

	p[0].tex.x = t.x;
	p[0].tex.y = t.y + chrStep;

	p[1].tex.x = t.x;
	p[1].tex.y = t.y;

	p[2].tex.x = t.x + chrStep;
	p[2].tex.y = t.y + chrStep;

	p[3].tex.x = t.x + chrStep;
	p[3].tex.y = t.y;

	TransformAndProjectPoints4(p);
}

// *****************************************************************************************************************
// *****************************************************************************************************************
[maxvertexcount(64)]
void MainGS( triangle S_PixelInput input[3], inout TriangleStream<S_PixelInput> OutputStream )
{	
	S_PixelInput p[4];

	uint elementIndex = input[0].iid;
	uint elementFlags = elements[elementIndex].flags;
	uint elementType = elementFlags & 0xf;

	p[0].iid = elementFlags;
	p[1].iid = elementFlags;
	p[2].iid = elementFlags;
	p[3].iid = elementFlags;

	if( elementType == 0 ) // box
	{
		MakeBoxOutput(elementIndex, p);
		OutputStream.Append(p[0]);	OutputStream.Append(p[1]);	OutputStream.Append(p[2]);	OutputStream.Append(p[3]);	OutputStream.RestartStrip();
	}
	else if( elementType == 1 ) // rectangle
	{
		// rectangle corners
		float2 lb,lt,rb,rt;
		GetRectangleCorners(elementIndex, lb,lt,rb,rt);
		// thickness
		float tx = elements[elementIndex].txy.x;
		float ty = elements[elementIndex].txy.y;

		// top line
		MakeSideLineBox( float2(lt.x+tx, lt.y), rt.x-tx, lt.y + ty, elements[elementIndex].color, p);
		OutputStream.Append(p[0]);	OutputStream.Append(p[1]);	OutputStream.Append(p[2]);	OutputStream.Append(p[3]);	OutputStream.RestartStrip();
		// bottom line
		MakeSideLineBox( float2(lt.x+tx, lb.y-ty), rt.x-tx, lb.y, elements[elementIndex].color, p);
		OutputStream.Append(p[0]);	OutputStream.Append(p[1]);	OutputStream.Append(p[2]);	OutputStream.Append(p[3]);	OutputStream.RestartStrip();
		// left line
		MakeSideLineBox( lt, lt.x + tx, lb.y, elements[elementIndex].color, p);
		OutputStream.Append(p[0]);	OutputStream.Append(p[1]);	OutputStream.Append(p[2]);	OutputStream.Append(p[3]);	OutputStream.RestartStrip();

		// right line
		MakeSideLineBox( float2(rt.x-tx, rt.y), rt.x, rb.y, elements[elementIndex].color, p);
		OutputStream.Append(p[0]);	OutputStream.Append(p[1]);	OutputStream.Append(p[2]);	OutputStream.Append(p[3]);	OutputStream.RestartStrip();
	}
	else if( elementType == 2 ) // text
	{
		uint strArray[16];
		GetStringArray(elementIndex, strArray);
		float2 pt = elements[elementIndex].pxy;
		float2 sz = elements[elementIndex].psxy;
		float4 color = elements[elementIndex].color;
		float shrinkx = elements[elementIndex].reserved.x;

		[fastopt]
		for(uint i = 0; i < 16; i++)
		if(strArray[i])
		{
			MakeCharBox(pt, sz, strArray[i], color, p);

			OutputStream.Append(p[0]);	OutputStream.Append(p[1]);	OutputStream.Append(p[2]);	OutputStream.Append(p[3]);	OutputStream.RestartStrip();

			pt.x += sz.x;
			pt.x += shrinkx;
		}
	}
}

// *****************************************************************************************************************
// *****************************************************************************************************************
Texture2D shaderTexture[8];

float4 MainPS(S_PixelInput input) : SV_TARGET
{
	uint elementFlags = input.iid;
	uint elementType = elementFlags & 0xf;
	uint textureType = (elementFlags >> 4) & 0xffff;
	const float whiteTresh = 0.5;

    float4 textureColor = {1,1,1,1};
	if(textureType & 1)
		textureColor = shaderTexture[0].Sample(samplerLinearMirror, input.tex);
	if(textureType & 1 << 1)
		textureColor = shaderTexture[1].Sample(samplerLinearMirror, input.tex);
	if(textureType & 1 << 2)
		textureColor = shaderTexture[2].Sample(samplerLinearMirror, input.tex);
	if(textureType & 1 << 3)
		textureColor = shaderTexture[3].Sample(samplerLinearMirror, input.tex);
	if(textureType & 1 << 4)
		textureColor = shaderTexture[4].Sample(samplerLinearMirror, input.tex);
	if(textureType & 1 << 5)
		textureColor = shaderTexture[5].Sample(samplerLinearMirror, input.tex);
	if(textureType & 1 << 6)
		textureColor = shaderTexture[6].Sample(samplerLinearMirror, input.tex);
	if(textureType & 1 << 7)
		textureColor = shaderTexture[7].Sample(samplerLinearMirror, input.tex);

	if(elementType == 2)
	{
		if(!(textureColor.x > whiteTresh && textureColor.y > whiteTresh && textureColor.z > whiteTresh)) discard;
		else textureColor = float4(1,1,1,1);
	}

	float4 color=saturate(textureColor * input.color);

    return color;
}
