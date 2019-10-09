#pragma once

#ifndef AE_RENDER_SYS_RESOURCE_VB_H
#define AE_RENDER_SYS_RESOURCE_VB_H

#ifdef AE_RENDER_DX11
	#include "../dx11/resources/resource_vb_dx11.h"
	using ae::render::dx11::C_VertexBufferResource;
#else
	#error No resource vertex buffer available !
#endif // #ifdef AE_RENDER_DX11

#include "../../../resource_sys/resource_sys.h"
#include "../../../math_sys/types.h"

namespace ae { namespace render {

	struct S_VertexBufferResource : public ::ae::resource::S_Resource
	{
		typedef ae::u32 T_VertexFormat;
		static const T_VertexFormat VF_POSITION = 1;
		static const T_VertexFormat VF_UV0		= 2;
		static const T_VertexFormat VF_NORMAL	= 4;
		static const T_VertexFormat VF_COLOR0	= 8;
		T_VertexFormat vertexFormat;
		ae::u32 vertextSize, numVertices;

		struct S_Vertex
		{
			ae::math::float3 pos;
			ae::math::float2 uv;
		};

		C_VertexBufferResource m_Buffer;
	};

} } // namespace ae { namespace render {

#endif // #ifdef AE_RENDER_SYS_RESOURCE_VB_H
