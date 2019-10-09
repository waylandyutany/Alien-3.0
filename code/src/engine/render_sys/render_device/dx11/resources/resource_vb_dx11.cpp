#include "StdAfx.h"

#include "resource_vb_dx11.h"

namespace ae { namespace render { namespace dx11 {

C_VertexBufferResource::C_VertexBufferResource() :
	m_Buffer(0),
	m_Stride(0)
{
}

void C_VertexBufferResource::Activate(ID3D11DeviceContext* context)
{
	unsigned int stride;
	unsigned int offset;
	stride = m_Stride; 
	offset = 0;

	context->IASetVertexBuffers(0, 1, &m_Buffer, &stride, &offset);
}

void C_VertexBufferResource::Update(ID3D11DeviceContext* context, char* buffer, ae::u32 bufferSize)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = context->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, buffer, bufferSize);
	context->Unmap(m_Buffer, 0);
}

} } } // namespace ae { namespace render { namespace dx11 {
