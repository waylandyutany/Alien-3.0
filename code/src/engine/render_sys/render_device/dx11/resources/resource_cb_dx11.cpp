#include "StdAfx.h"

#include "resource_cb_dx11.h"

namespace ae { namespace render { namespace dx11 {

C_ConstantBufferResource::C_ConstantBufferResource() :
	m_Flags(E_BF_NONE),
	m_SlotIndex(E_BSI_None),
	m_BufferSize(0),
	m_Buffer(0)
{
}

void C_ConstantBufferResource::Update(ID3D11DeviceContext* context, void* data, size_t dataSize)
{
	AE_ASSERT(m_BufferSize == dataSize && "Buffer has different size then data buffer !");
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = context->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, data, dataSize);
	context->Unmap(m_Buffer, 0);
}

void C_ConstantBufferResource::Activate(ID3D11DeviceContext* context)
{
	if(m_Flags & E_BF_VS)
		context->VSSetConstantBuffers(m_SlotIndex, 1, &m_Buffer);
	if(m_Flags & E_BF_PS)
		context->PSSetConstantBuffers(m_SlotIndex, 1, &m_Buffer);
	if(m_Flags & E_BF_GS)
		context->GSSetConstantBuffers(m_SlotIndex, 1, &m_Buffer);
	if(m_Flags & E_BF_CS)
		context->CSSetConstantBuffers(m_SlotIndex, 1, &m_Buffer);
	if(m_Flags & E_BF_HS)
		context->HSSetConstantBuffers(m_SlotIndex, 1, &m_Buffer);
	if(m_Flags & E_BF_DS)
		context->DSSetConstantBuffers(m_SlotIndex, 1, &m_Buffer);
}

} } } // namespace ae { namespace render { namespace dx11 {
