#pragma once

#ifndef AE_RENDER_SYS_RENDER_RESOURCES_DX11_H
#define AE_RENDER_SYS_RENDER_RESOURCES_DX11_H

#include "resources/resource_texture_dx11.h"
#include "resources/resource_effect_dx11.h"
#include "resources/resource_ib_dx11.h"
#include "resources/resource_vb_dx11.h"
#include "resources/resource_cb_dx11.h"
#include "resources/resource_structured_buffer_dx11.h"
#include "resources/resource_input_layout_dx11.h"
#include "constants_dx11.h"

#include "../resources/resource_vb.h"

namespace ae { namespace base {
	class C_IncrementalAllocator;
} } // namespace ae { namespace base {

namespace ae { namespace render {
	class C_RenderResources;
	struct S_VertexBufferResource;
	struct S_EffectResource;

} } // namespace ae { namespace render {

namespace ae { namespace render { namespace dx11 {

	class C_RenderDevice;

	class C_RenderResources
	{
		C_RenderDevice* m_RenderDevice;

		C_RenderResources(C_RenderDevice*);
		~C_RenderResources();
// *****************************************************************************************************************
		friend class ae::render::C_RenderResources;
		NOCOPY_CLASS(C_RenderResources);
		C_RenderResources();
	public:
// *****************************************************************************************************************
		bool CreateEffectResource(S_EffectResource&, C_EffectResource&);
		void DestroyEffectResource(C_EffectResource&);
// *****************************************************************************************************************
		bool CreateIndexBufferResource(void* buff, size_t buffSize, C_IndexBufferResource::E_Index_Buffer_Type, C_IndexBufferResource& rsc);
		void DestroyIndexBufferResource(C_IndexBufferResource& rsc);
// *****************************************************************************************************************
		bool CreateVertexBufferResource(void* buff, size_t buffSize, ae::u32 stride, bool bDynamic, C_VertexBufferResource& rsc);
		void DestroyVertexBufferResource(C_VertexBufferResource& rsc);
// *****************************************************************************************************************
		bool CreateConstantBufferResource(void* buff, size_t buffSize, T_BufferFlags flags, T_BufferSlotIndex slotIndex, C_ConstantBufferResource& rsc);
		bool CreateConstantBufferResource(size_t buffSize, T_BufferFlags flags, T_BufferSlotIndex slotIndex, C_ConstantBufferResource& rsc);
		void DestroyConstantBufferResource(C_ConstantBufferResource& rsc);
// *****************************************************************************************************************
		bool CreateStructuredBufferResource(void* buff, ae::u32 numStructs, ae::u32 structSize, T_BufferFlags flags, T_BufferSlotIndex slotIndex, C_StructuredBufferResource& rsc);
		void DestroyStructuredBufferResource(C_StructuredBufferResource& rsc);
// *****************************************************************************************************************
		bool CreateInputLayout(S_VertexBufferResource::T_VertexFormat, S_EffectResource&, C_InputLayoutResource&);
		bool CreateInputLayout(S_VertexBufferResource&, S_EffectResource&, C_InputLayoutResource&);
		void DestroyInputLayout(C_InputLayoutResource& rsc);
// *****************************************************************************************************************
		bool CreateTextureResource(void* buff, size_t buffSize, C_TextureResource&);
		void DestroyTextureResource(C_TextureResource&);
// *****************************************************************************************************************
		bool ImportTextureResourceToMemory(char* pTextureBuffer, size_t textureBufferSize, ae::base::C_IncrementalAllocator& outputAllocator);
// *****************************************************************************************************************
	};

} } } // namespace ae { namespace render { namespace dx11 {

#endif // #ifdef AE_RENDER_SYS_RENDER_RESOURCES_DX11_H
