#pragma once

#ifndef AE_RENDER_SYS_MESH_RESOURCE_H
#define AE_RENDER_SYS_MESH_RESOURCE_H

#include "../../../math_sys/types.h"
#include "../../../base_sys/hash.h"

#include "../dx11/resources/resource_cb_dx11.h"
#include "../dx11/resources/resource_input_layout_dx11.h"
using ae::render::dx11::C_ConstantBufferResource;
using ae::render::dx11::C_InputLayoutResource;

namespace ae { namespace render {

// ************************************************************************************
	struct S_VertexBufferResource;
	struct S_IndexBufferResource;
	struct S_EffectResource;
	struct S_TextureResource;
// ************************************************************************************

	struct S_MeshResource
	{
		struct S_Material
		{
			ae::base::S_HashID hid;

			math::float4 diffuse;
			ae::base::S_HashID diffuseTexture;

			S_TextureResource* pDiffuseTexture;
			C_ConstantBufferResource m_CBuffer;

			S_Material* next;
		};
// ************************************************************************************
		struct S_Geometry
		{
			struct S_Triangles
			{
				S_Material* material;

				ae::u32 vertexOffset;

				ae::u32 indexOffset;
				ae::u32 indexCount;

				S_Triangles* next;
			};

			ae::base::S_HashID hid;
			ae::math::box3d boundingBox;
			S_Triangles* triangles;
			S_Geometry* next;
		};
// ************************************************************************************
		S_Material* materials;
		S_Geometry* geometries;
// ************************************************************************************
		S_VertexBufferResource* m_pVBResource;
		S_IndexBufferResource* m_pIBResource;
		S_EffectResource* m_pEffect;
		C_InputLayoutResource m_InputLayoutResource;
// ************************************************************************************
	};

// ************************************************************************************
// ************************************************************************************

} } // namespace ae { namespace render {

#endif // #ifdef AE_RENDER_SYS_MESH_RESOURCE_H
