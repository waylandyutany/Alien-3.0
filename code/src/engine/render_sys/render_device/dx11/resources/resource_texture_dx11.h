#pragma once

#ifndef AE_RENDER_SYS_RESOURCE_TEXTURE_DX11_H
#define AE_RENDER_SYS_RESOURCE_TEXTURE_DX11_H

namespace ae { namespace render { namespace dx11 {

	class C_RenderResources;

	class C_TextureResource
	{
		ID3D11ShaderResourceView *m_TextureResourceView;

		NOCOPY_CLASS(C_TextureResource);
		friend class C_RenderResources;
	public:
		C_TextureResource();
		void Activate(ID3D11DeviceContext*, ae::u32 slotIndex);
	};

} } } // namespace ae { namespace render { namespace dx11 {

#endif // #ifdef AE_RENDER_SYS_RESOURCE_TEXTURE_DX11_H
