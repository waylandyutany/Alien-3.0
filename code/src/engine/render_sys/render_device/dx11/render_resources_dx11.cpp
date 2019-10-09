#include "StdAfx.h"

#include "render_resources_dx11.h"
#include "render_device_dx11.h"
#include "utils_dx11.h"

#include "../../render_device/resources/resource_vb.h"
#include "../../render_device/resources/resource_effect.h"

#include "../../../base_sys/memory/allocators/incremental_allocator.h"
#include "../../../_sys/templates.h"

namespace ae { namespace render { namespace dx11 {

C_RenderResources::C_RenderResources(C_RenderDevice* device) :
	m_RenderDevice(device)
{
}

C_RenderResources::~C_RenderResources()
{
}

// *****************************************************************************************************************
// *****************************************************************************************************************

bool C_RenderResources::CreateEffectResource(S_EffectResource& effect, C_EffectResource& rsc)
{
	bool bRet = true;
	HRESULT result;

	if(effect.m_ShaderBuffers[0].pBuffer) {
		result = m_RenderDevice->GetDevice()->CreateVertexShader(effect.m_ShaderBuffers[0].pBuffer, effect.m_ShaderBuffers[0].bufferSize, NULL, &rsc.m_pVertexShader);
		if(result != S_OK) bRet = false;
	}

	if(effect.m_ShaderBuffers[1].pBuffer) {
		result = m_RenderDevice->GetDevice()->CreateGeometryShader(effect.m_ShaderBuffers[1].pBuffer, effect.m_ShaderBuffers[1].bufferSize, NULL, &rsc.m_pGeometryShader);
		if(result != S_OK) bRet = false;
	}

	if(effect.m_ShaderBuffers[2].pBuffer) {
		result = m_RenderDevice->GetDevice()->CreatePixelShader(effect.m_ShaderBuffers[2].pBuffer, effect.m_ShaderBuffers[2].bufferSize, NULL, &rsc.m_pPixelShader);
		if(result != S_OK) bRet = false;
	}

	if(effect.m_ShaderBuffers[3].pBuffer) {
		result = m_RenderDevice->GetDevice()->CreateComputeShader(effect.m_ShaderBuffers[3].pBuffer, effect.m_ShaderBuffers[3].bufferSize, NULL, &rsc.m_pComputeShader);
		if(result != S_OK) bRet = false;
	}

	if(effect.m_ShaderBuffers[4].pBuffer) {
		result = m_RenderDevice->GetDevice()->CreateDomainShader(effect.m_ShaderBuffers[4].pBuffer, effect.m_ShaderBuffers[4].bufferSize, NULL, &rsc.m_pDomainShader);
		if(result != S_OK) bRet = false;
	}

	if(effect.m_ShaderBuffers[5].pBuffer) {
		result = m_RenderDevice->GetDevice()->CreateHullShader(effect.m_ShaderBuffers[5].pBuffer, effect.m_ShaderBuffers[5].bufferSize, NULL, &rsc.m_pHullShader);
		if(result != S_OK) bRet = false;
	}

	return bRet;
}

void C_RenderResources::DestroyEffectResource(C_EffectResource& rsc)
{
	ae::SafeRelease(rsc.m_pVertexShader);
	ae::SafeRelease(rsc.m_pPixelShader);
	ae::SafeRelease(rsc.m_pGeometryShader);
	ae::SafeRelease(rsc.m_pComputeShader);
	ae::SafeRelease(rsc.m_pDomainShader);
	ae::SafeRelease(rsc.m_pHullShader);
}

// *****************************************************************************************************************
// *****************************************************************************************************************

bool C_RenderResources::CreateIndexBufferResource(void* buff, size_t buffSize, C_IndexBufferResource::E_Index_Buffer_Type type, C_IndexBufferResource& rsc)
{
	HRESULT result;
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA bufferData;

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = (UINT)buffSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	bufferData.pSysMem = buff;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	result = m_RenderDevice->GetDevice()->CreateBuffer(&bufferDesc, &bufferData, &rsc.m_Buffer);
	rsc.m_Type = type;

	return true;
}

void C_RenderResources::DestroyIndexBufferResource(C_IndexBufferResource& rsc)
{
	ae::SafeRelease(rsc.m_Buffer);
}

// *****************************************************************************************************************
// *****************************************************************************************************************

bool C_RenderResources::CreateVertexBufferResource(void* buff, size_t buffSize, ae::u32 stride, bool bDynamic, C_VertexBufferResource& rsc)
{
	HRESULT result;
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA bufferData;

	bufferDesc.Usage = bDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = (UINT)buffSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = bDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	bufferData.pSysMem = buff;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	result = m_RenderDevice->GetDevice()->CreateBuffer(&bufferDesc, bDynamic ? 0 : &bufferData, &rsc.m_Buffer);
	rsc.m_Stride = stride;

	return true;
}

void C_RenderResources::DestroyVertexBufferResource(C_VertexBufferResource& rsc)
{
	ae::SafeRelease(rsc.m_Buffer);
}

// *****************************************************************************************************************
// *****************************************************************************************************************

bool C_RenderResources::CreateConstantBufferResource(size_t buffSize, T_BufferFlags flags, T_BufferSlotIndex slotIndex, C_ConstantBufferResource& rsc)
{
	HRESULT result;
	D3D11_BUFFER_DESC bufferDesc;

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)buffSize;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	result = m_RenderDevice->GetDevice()->CreateBuffer(&bufferDesc, 0, &rsc.m_Buffer);
	rsc.m_Flags = flags;
	rsc.m_SlotIndex = slotIndex;
	rsc.m_BufferSize = buffSize;

	return true;
}

bool C_RenderResources::CreateConstantBufferResource(void* buff, size_t buffSize, T_BufferFlags flags, T_BufferSlotIndex slotIndex, C_ConstantBufferResource& rsc)
{
	HRESULT result;
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA data;

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)buffSize;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	data.pSysMem = buff;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	result = m_RenderDevice->GetDevice()->CreateBuffer(&bufferDesc, buff ? &data : 0, &rsc.m_Buffer);
	rsc.m_Flags = flags;
	rsc.m_SlotIndex = slotIndex;
	rsc.m_BufferSize = buffSize;

	return true;
}

void C_RenderResources::DestroyConstantBufferResource(C_ConstantBufferResource& rsc)
{
	ae::SafeRelease(rsc.m_Buffer);
}

// *****************************************************************************************************************
// *****************************************************************************************************************
bool C_RenderResources::CreateStructuredBufferResource(void* buff, ae::u32 numStructs, ae::u32 structSize, T_BufferFlags flags, T_BufferSlotIndex slotIndex, C_StructuredBufferResource& rsc)
{
	HRESULT result;
    D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA bufferData;
    ZeroMemory( &bufferDesc, sizeof(bufferDesc));
    ZeroMemory( &bufferData, sizeof(bufferData));
    bufferDesc.ByteWidth = numStructs * structSize;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
//    bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bufferDesc.StructureByteStride = structSize;

	bufferData.pSysMem = buff;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	result = m_RenderDevice->GetDevice()->CreateBuffer( &bufferDesc, buff ? &bufferData : 0, &rsc.m_Buffer);
	if(result != S_OK) return false;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvbufferDesc;
    ZeroMemory( &srvbufferDesc, sizeof(srvbufferDesc) );
    srvbufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvbufferDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srvbufferDesc.Buffer.ElementWidth = numStructs;
	result = m_RenderDevice->GetDevice()->CreateShaderResourceView( rsc.m_Buffer, &srvbufferDesc, &rsc.m_BufferSRV );
	if(result != S_OK)
	{
		ae::SafeRelease(rsc.m_Buffer);
		return false;
	}

	rsc.m_Flags = flags;
	rsc.m_SlotIndex = slotIndex;

	return true;
}

void C_RenderResources::DestroyStructuredBufferResource(C_StructuredBufferResource& rsc)
{
	ae::SafeRelease(rsc.m_Buffer);
	ae::SafeRelease(rsc.m_BufferSRV);
}

// *****************************************************************************************************************
// *****************************************************************************************************************

bool C_RenderResources::CreateInputLayout(S_VertexBufferResource::T_VertexFormat vertexFormat, S_EffectResource& effect, C_InputLayoutResource& rsc)
{
	ae::u32 numElements = 0;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];

	if(vertexFormat & S_VertexBufferResource::VF_POSITION)
	{
		polygonLayout[numElements].SemanticName = "POSITION";
		polygonLayout[numElements].SemanticIndex = 0;
		polygonLayout[numElements].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[numElements].InputSlot = 0;
		polygonLayout[numElements].AlignedByteOffset = 0;
		polygonLayout[numElements].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[numElements].InstanceDataStepRate = 0;
		numElements++;
	}
	if(vertexFormat & S_VertexBufferResource::VF_UV0)
	{
		polygonLayout[numElements].SemanticName = "TEXCOORD";
		polygonLayout[numElements].SemanticIndex = 0;
		polygonLayout[numElements].Format = DXGI_FORMAT_R32G32_FLOAT;
		polygonLayout[numElements].InputSlot = 0;
		polygonLayout[numElements].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[numElements].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[numElements].InstanceDataStepRate = 0;
		numElements++;
	}

	if(vertexFormat & S_VertexBufferResource::VF_COLOR0)
	{
		polygonLayout[numElements].SemanticName = "COLOR";
		polygonLayout[numElements].SemanticIndex = 0;
		polygonLayout[numElements].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		polygonLayout[numElements].InputSlot = 0;
		polygonLayout[numElements].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[numElements].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[numElements].InstanceDataStepRate = 0;
		numElements++;
	}

	char* pVS = (char*)effect.m_ShaderBuffers[0].pBuffer;
	const size_t sizeVS = effect.m_ShaderBuffers[0].bufferSize;

	HRESULT result;
	result = m_RenderDevice->GetDevice()->CreateInputLayout(polygonLayout, numElements, pVS, sizeVS, &rsc.m_Layout);
	return true;
}

bool C_RenderResources::CreateInputLayout(S_VertexBufferResource& rscVB, S_EffectResource& rscVS, C_InputLayoutResource& rsc)
{
	return CreateInputLayout(rscVB.vertexFormat, rscVS, rsc);
}

void C_RenderResources::DestroyInputLayout(C_InputLayoutResource& rsc)
{
	ae::SafeRelease(rsc.m_Layout);
}

// *****************************************************************************************************************
// *****************************************************************************************************************

bool C_RenderResources::CreateTextureResource(void* buff, size_t buffSize, C_TextureResource& rsc)
{
	D3DX11_IMAGE_LOAD_INFO imageLoadInfo;

	imageLoadInfo.Width = D3DX11_DEFAULT;
    imageLoadInfo.Height = D3DX11_DEFAULT;
    imageLoadInfo.Depth = D3DX11_DEFAULT;
    imageLoadInfo.FirstMipLevel = 1;
    imageLoadInfo.MipLevels = 1;
    imageLoadInfo.Usage = (D3D11_USAGE) D3DX11_DEFAULT;
    imageLoadInfo.BindFlags = D3DX11_DEFAULT;
    imageLoadInfo.CpuAccessFlags = D3DX11_DEFAULT;
    imageLoadInfo.MiscFlags = D3DX11_DEFAULT;
    imageLoadInfo.Format = DXGI_FORMAT_FROM_FILE;
    imageLoadInfo.Filter = D3DX11_DEFAULT;
    imageLoadInfo.MipFilter = D3DX11_DEFAULT;
    imageLoadInfo.pSrcInfo = NULL;

	HRESULT result = D3DX11CreateShaderResourceViewFromMemory(m_RenderDevice->GetDevice(), buff, buffSize, &imageLoadInfo, NULL, &rsc.m_TextureResourceView, NULL);
	return result == S_OK ? true : false;
}

void C_RenderResources::DestroyTextureResource(C_TextureResource& rsc)
{
	ae::SafeRelease(rsc.m_TextureResourceView);
}

// *****************************************************************************************************************
// *****************************************************************************************************************

bool C_RenderResources::ImportTextureResourceToMemory(char* pTextureBuffer, size_t textureBufferSize, ae::base::C_IncrementalAllocator& outputAllocator)
{
	HRESULT result;
	ID3D11Resource* textureResource;
	ID3D10Blob* ppDestBuf;
	D3DX11_IMAGE_LOAD_INFO imageLoadInfo;

	imageLoadInfo.Width = D3DX11_DEFAULT;
    imageLoadInfo.Height = D3DX11_DEFAULT;
    imageLoadInfo.Depth = D3DX11_DEFAULT;
    imageLoadInfo.FirstMipLevel = 1;
    imageLoadInfo.MipLevels = 1;
    imageLoadInfo.Usage = (D3D11_USAGE) D3DX11_DEFAULT;
    imageLoadInfo.BindFlags = D3DX11_DEFAULT;
    imageLoadInfo.CpuAccessFlags = D3DX11_DEFAULT;
    imageLoadInfo.MiscFlags = D3DX11_DEFAULT;
    imageLoadInfo.Format = DXGI_FORMAT_FROM_FILE;
    imageLoadInfo.Filter = D3DX11_DEFAULT;
    imageLoadInfo.MipFilter = D3DX11_DEFAULT;
    imageLoadInfo.pSrcInfo = NULL;

	result = D3DX11CreateTextureFromMemory(m_RenderDevice->GetDevice(), pTextureBuffer, textureBufferSize, &imageLoadInfo, NULL, &textureResource, NULL);
	if(result != S_OK)
		return false;

	result = D3DX11SaveTextureToMemory(m_RenderDevice->GetContext(), textureResource, D3DX11_IFF_DDS, &ppDestBuf, 0);
	if(result != S_OK)
	{
		ae::SafeRelease(textureResource);
		return false;
	}

	char* outpuBuffer = outputAllocator.Alloc(ppDestBuf->GetBufferSize());
	memcpy(outpuBuffer, ppDestBuf->GetBufferPointer(), ppDestBuf->GetBufferSize());

	ae::SafeRelease(ppDestBuf);

	return true;
}

// *****************************************************************************************************************
// *****************************************************************************************************************

} } } // namespace ae { namespace render { namespace dx11 {
