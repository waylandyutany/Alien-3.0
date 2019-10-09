#pragma once

#ifndef AE_RENDER_SYS_RESOURCE_IB_DX11_H
#define AE_RENDER_SYS_RESOURCE_IB_DX11_H

namespace ae { namespace render { namespace dx11 {

	class C_RenderResources;

	class C_IndexBufferResource
	{
	public:
		enum E_Index_Buffer_Type
		{
			E_IBT_16Bit,
			E_IBT_32Bit
		};
	private:
		ID3D11Buffer *m_Buffer;
		E_Index_Buffer_Type m_Type;
		NOCOPY_CLASS(C_IndexBufferResource);
		friend class C_RenderResources;
	public:
		C_IndexBufferResource();
		void Activate(ID3D11DeviceContext*);
	};

} } } // namespace ae { namespace render { namespace dx11 {

#endif // #ifdef AE_RENDER_SYS_RESOURCE_IB_DX11_H
