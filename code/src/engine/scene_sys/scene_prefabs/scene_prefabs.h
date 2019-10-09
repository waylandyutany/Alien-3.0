#pragma once

#ifndef AE_SCENE_SYS_SCENE_PREFABS_H
#define AE_SCENE_SYS_SCENE_PREFABS_H

#include "../../base_sys/hash.h"
#include "../../render_sys/render_device/resources/resource_mesh.h"
#include "../../render_sys/render_device/dx11/resources/resource_structured_buffer_dx11.h"

#include "../../physic_sys/resources/resource_collision.h"

namespace ae { namespace scene {

	struct S_Instance
	{
		ae::math::float4x4 worldMatrix;
		ae::math::float4x4 invWorldMatrix; // @todo not needed atm, remove it
	};

	enum E_ScenePrefab_Type
	{
		E_SINT_Instance,
		E_SINT_LOD,
		E_SINT_MGeometry,
		E_SINT_CGeometry,
	};

	struct S_ScenePrefabBase
	{
		E_ScenePrefab_Type type;
		S_ScenePrefabBase* next;
		S_ScenePrefabBase* child;
	};

	struct S_ScenePrefab : public S_ScenePrefabBase
	{
		ae::base::S_HashID hid;
	};

	struct S_ScenePrefab_LOD : public S_ScenePrefabBase
	{
	};

	struct S_ScenePrefab_MGeometry : public S_ScenePrefabBase
	{
		ae::render::S_MeshResource* pMesh;
		ae::render::S_MeshResource::S_Geometry* pGeometry;
		ae::u32 numInstances;
		S_Instance* instances;
		ae::render::dx11::C_StructuredBufferResource instancesCB;
		const char* elementPath;
		bool bCastShadows;
	};

	struct S_ScenePrefab_CGeometry : public S_ScenePrefabBase
	{
		ae::scene::S_CollisionResource* pCollision; // still need to initialize when activating resource
		ae::scene::S_CollisionResource::S_Geometry* pGeometry;
		ae::u32 numInstances;
		S_Instance* instances;

		const char* elementPath;
	};

	S_ScenePrefab_MGeometry* FindGeometryNode(S_ScenePrefabBase* pNode, const char* geometryName, ae::u32 numInstancies);
} } // namespace ae { namespace scene {

#endif // #ifdef AE_SCENE_SYS_SCENE_PREFABS_H
