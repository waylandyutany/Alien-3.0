#include "StdAfx.h"

#include "resource_structured_buffer_dx11.h"

namespace ae { namespace render { namespace dx11 {

C_StructuredBufferResource::C_StructuredBufferResource() :
	m_Flags(E_BF_NONE),
	m_SlotIndex(E_BSI_None),
	m_BufferSize(0),
	m_Buffer(0),
	m_BufferSRV(0)
{
}

void C_StructuredBufferResource::Update(ID3D11DeviceContext* context, void* data, ae::u32 /*dataSize*/)
{
    context->UpdateSubresource( m_Buffer, 0, NULL, data, 0, 0 );
}

void C_StructuredBufferResource::Activate(ID3D11DeviceContext* context)
{
	if(m_Flags & E_BF_VS)
		context->VSSetShaderResources(m_SlotIndex, 1, &m_BufferSRV);
	if(m_Flags & E_BF_PS)
		context->PSSetShaderResources(m_SlotIndex, 1, &m_BufferSRV);
	if(m_Flags & E_BF_GS)
		context->GSSetShaderResources(m_SlotIndex, 1, &m_BufferSRV);
	if(m_Flags & E_BF_CS)
		context->CSSetShaderResources(m_SlotIndex, 1, &m_BufferSRV);
	if(m_Flags & E_BF_HS)
		context->HSSetShaderResources(m_SlotIndex, 1, &m_BufferSRV);
	if(m_Flags & E_BF_DS)
		context->DSSetShaderResources(m_SlotIndex, 1, &m_BufferSRV);
}

} } } // namespace ae { namespace render { namespace dx11 {
