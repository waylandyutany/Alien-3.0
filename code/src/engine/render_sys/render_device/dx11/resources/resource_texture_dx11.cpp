#include "StdAfx.h"

#include "resource_texture_dx11.h"

namespace ae { namespace render { namespace dx11 {

C_TextureResource::C_TextureResource() :
	m_TextureResourceView(0)
{
}

void C_TextureResource::Activate(ID3D11DeviceContext* context, ae::u32 slotIndex)
{
	context->PSSetShaderResources(slotIndex, 1, &m_TextureResourceView);
}

} } } // namespace ae { namespace render { namespace dx11 {
