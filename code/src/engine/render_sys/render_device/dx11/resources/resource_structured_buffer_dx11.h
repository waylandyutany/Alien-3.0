#pragma once

#ifndef AE_RENDER_SYS_RESOURCE_STRUCTURED_BUFFER_DX11_H
#define AE_RENDER_SYS_RESOURCE_STRUCTURED_BUFFER_DX11_H

#include "../constants_dx11.h"

namespace ae { namespace render { namespace dx11 {

	class C_RenderResources;
	
	class C_StructuredBufferResource
	{
		T_BufferFlags		m_Flags;
		T_BufferSlotIndex	m_SlotIndex;

		ae::u32 m_BufferSize;

		ID3D11Buffer *m_Buffer;
		ID3D11ShaderResourceView* m_BufferSRV;

		NOCOPY_CLASS(C_StructuredBufferResource);
		friend class C_RenderResources;
	public:
		C_StructuredBufferResource();
		void Update(ID3D11DeviceContext* context, void* data, ae::u32 dataSize);
		void Activate(ID3D11DeviceContext* context);
	};

} } } // namespace ae { namespace render { namespace dx11 {

#endif // #ifdef AE_RENDER_SYS_RESOURCE_STRUCTURED_BUFFER_DX11_H
