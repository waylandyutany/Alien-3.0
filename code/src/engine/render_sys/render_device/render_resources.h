#pragma once

#ifndef AE_RENDER_SYS_RENDER_RESOURCES_H
#define AE_RENDER_SYS_RENDER_RESOURCES_H

#ifdef AE_RENDER_DX11
	#include "dx11/render_resources_dx11.h"
#else
	#error No render resources available !
#endif // #ifdef AE_RENDER_DX11

#include "../../math_sys/types.h"

namespace ae { namespace base {
	class C_IncrementalAllocator;
} } // namespace ae { namespace base {

namespace ae { namespace render {

	class C_RenderSys;
	class C_RenderDevice;

	struct S_InstanciesBuffer
	{
		S_InstanciesBuffer();
		void Clear();
		static const ae::u32 maxInstancies = 1024;
		ae::math::float4x4 instanciesMatricies[maxInstancies];
	};

	class C_RenderResources
	{
		friend class ae::render::C_RenderSys;
		dx11::C_RenderResources* m_Impl;
		C_RenderDevice* m_Device;
		S_InstanciesBuffer m_InstanciesBuffer;

		C_RenderResources(C_RenderDevice*);
		~C_RenderResources();

		NOCOPY_CLASS(C_RenderResources);
	public:
	#ifdef AE_RENDER_DX11
		dx11::C_RenderResources* GetImpl() { return m_Impl; }
	#endif // #ifdef AE_RENDER_DX11

		S_InstanciesBuffer& InstanciesBuffer() { return m_InstanciesBuffer; }
		bool ImportTextureResourceToMemory(char* pTextureBuffer, size_t textureBufferSize, ae::base::C_IncrementalAllocator& outputAllocator);
	};

} } // namespace ae { namespace render {

#endif // #ifdef AE_RENDER_SYS_RENDER_RESOURCES_H
