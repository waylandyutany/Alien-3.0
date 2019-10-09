#pragma once

#ifndef AE_RENDER_SYS_RESOURCE_CB_DX11_H
#define AE_RENDER_SYS_RESOURCE_CB_DX11_H

#include "../constants_dx11.h"

namespace ae { namespace render { namespace dx11 {

	class C_RenderResources;
	
	class C_ConstantBufferResource
	{
		T_BufferFlags		m_Flags;
		T_BufferSlotIndex	m_SlotIndex;

		size_t m_BufferSize;
		ID3D11Buffer *m_Buffer;

		NOCOPY_CLASS(C_ConstantBufferResource);
		friend class C_RenderResources;

	public:
		C_ConstantBufferResource();
		void Update(ID3D11DeviceContext* context, void* data, size_t dataSize);
		void Activate(ID3D11DeviceContext* context);
	};

} } } // namespace ae { namespace render { namespace dx11 {

#endif // #ifdef AE_RENDER_SYS_RESOURCE_CB_DX11_H
