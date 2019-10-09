#include "StdAfx.h"

#include "resource_ib_dx11.h"

namespace ae { namespace render { namespace dx11 {

C_IndexBufferResource::C_IndexBufferResource() :
	m_Buffer(0)
{
}

void C_IndexBufferResource::Activate(ID3D11DeviceContext* context)
{
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetIndexBuffer(m_Buffer, m_Type == E_IBT_16Bit ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
}

} } } // namespace ae { namespace render { namespace dx11 {
