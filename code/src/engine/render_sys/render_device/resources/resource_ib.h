#pragma once

#ifndef AE_RENDER_SYS_RESOURCE_IB_H
#define AE_RENDER_SYS_RESOURCE_IB_H

#ifdef AE_RENDER_DX11
	#include "../dx11/resources/resource_ib_dx11.h"
	using ae::render::dx11::C_IndexBufferResource;
#else
	#error index buffer available !
#endif // #ifdef AE_RENDER_DX11

#include "../../../resource_sys/resource_sys.h"

namespace ae { namespace render {

	struct S_IndexBufferResource : public ::ae::resource::S_Resource
	{
		C_IndexBufferResource::E_Index_Buffer_Type m_IndexBufferType;
		C_IndexBufferResource m_Buffer;
	};

} } // namespace ae { namespace render {

#endif // #ifdef AE_RENDER_SYS_RESOURCE_IB_H
