#pragma once

#ifndef AE_RENDER_SYS_RESOURCE_EFFECT_H
#define AE_RENDER_SYS_RESOURCE_EFFECT_H

#ifdef AE_RENDER_DX11
	#include "../dx11/resources/resource_effect_dx11.h"
	using ae::render::dx11::C_EffectResource;
#else
	#error No effect resource available !
#endif // #ifdef AE_RENDER_DX11

#include "../../../resource_sys/resource_sys.h"

namespace ae { namespace render {

	struct S_EffectResource : public ::ae::resource::S_Resource
	{
		struct S_ShaderBuffer {
			void* pBuffer;
			size_t bufferSize;
		};
		S_ShaderBuffer m_ShaderBuffers[6];// VS,GS,PS,CS,DS,HS
		C_EffectResource m_Effect;
	};

} } // namespace ae { namespace render {

#endif // #ifdef AE_RENDER_SYS_RESOURCE_EFFECT_H
