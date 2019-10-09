#pragma once

#ifndef AE_RENDER_SYS_RESOURCE_VB_DX11_H
#define AE_RENDER_SYS_RESOURCE_VB_DX11_H

namespace ae { namespace render { namespace dx11 {

	class C_RenderResources;

	class C_VertexBufferResource
	{
		ID3D11Buffer *m_Buffer;
		ae::u32 m_Stride;

		NOCOPY_CLASS(C_VertexBufferResource);
		friend class C_RenderResources;
	public:
		C_VertexBufferResource();
		void Activate(ID3D11DeviceContext*);
		void Update(ID3D11DeviceContext*, char* buffer, ae::u32 bufferSize);
	};

} } } // namespace ae { namespace render { namespace dx11 {

#endif // #ifdef AE_RENDER_SYS_RESOURCE_VB_DX11_H
