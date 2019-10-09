#include "StdAfx.h"

#include "resource_input_layout_dx11.h"

namespace ae { namespace render { namespace dx11 {

C_InputLayoutResource::C_InputLayoutResource() :
	m_Layout(0)
{
}

void C_InputLayoutResource::Activate(ID3D11DeviceContext* context)
{
	context->IASetInputLayout(m_Layout);
}

} } } // namespace ae { namespace render { namespace dx11 {
