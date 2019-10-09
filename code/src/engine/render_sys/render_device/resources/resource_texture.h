#pragma once

#ifndef AE_RENDER_SYS_RESOURCE_TEXTURE_H
#define AE_RENDER_SYS_RESOURCE_TEXTURE_H

#ifdef AE_RENDER_DX11
	#include "../dx11/resources/resource_texture_dx11.h"
	using ae::render::dx11::C_TextureResource;
#else
	#error No resource vertex buffer available !
#endif // #ifdef AE_RENDER_DX11

#include "../../../resource_sys/resource_sys.h"

namespace ae { namespace render {

	struct S_TextureResource : public ::ae::resource::S_Resource
	{
		C_TextureResource m_TextureResource;
	};

} } // namespace ae { namespace render {

#endif // #ifdef AE_RENDER_SYS_RESOURCE_TEXTURE_H
