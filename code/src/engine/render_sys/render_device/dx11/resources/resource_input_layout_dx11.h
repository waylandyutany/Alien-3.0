#pragma once

#ifndef AE_RENDER_SYS_RESOURCE_INPUT_LAYOUT_DX11_H
#define AE_RENDER_SYS_RESOURCE_INPUT_LAYOUT_DX11_H

namespace ae { namespace render { namespace dx11 {

	class C_RenderResources;

	class C_InputLayoutResource
	{
		ID3D11InputLayout* m_Layout;

		NOCOPY_CLASS(C_InputLayoutResource);
		friend class C_RenderResources;
	public:
		C_InputLayoutResource();
		void Activate(ID3D11DeviceContext* context);
	};

} } } // namespace ae { namespace render { namespace dx11 {

#endif // #ifdef AE_RENDER_SYS_RESOURCE_INPUT_LAYOUT_DX11_H
