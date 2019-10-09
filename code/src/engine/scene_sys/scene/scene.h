#pragma once

#ifndef AE_SCENE_SYS_SCENE_H
#define AE_SCENE_SYS_SCENE_H

#include "../../base_sys/hash.h"
#include "../../math_sys/types.h"
#include "../../render_sys/render_scene/render_lights.h"

#include "../scene_prefabs/scene_prefabs.h"

namespace ae { namespace physic {
	class C_Actor;
	class C_DynamicActor;
} } // namespace ae { namespace physic {

namespace ae { namespace scene {

	class C_DirectionalLight;
	class C_PointLight;
	struct S_Static_Collision_Node;

	struct S_SceneNode
	{
		ae::base::S_HashID hid;
		ae::math::float4x4 worldMatrix;
		ae::math::float4x4 invWorldMatrix;
		C_ConstantBufferResource m_CBuffer; // constant buffer for world transformation matrix used for rendering

		S_ScenePrefabBase* pNode;
		const char* nodePath;

		S_SceneNode* next;
	};

	class C_DynamicSceneNode
	{
	public:
		C_ConstantBufferResource m_CBuffer; // constant buffer for world transformation matrix used for rendering
		ae::physic::C_DynamicActor* m_pDynamicActor;
		ae::render::S_MeshResource* m_pMesh;
		ae::render::S_MeshResource::S_Geometry* m_pGeometry;

		C_DynamicSceneNode(ae::physic::C_DynamicActor*,ae::render::S_MeshResource*,ae::render::S_MeshResource::S_Geometry*);
		virtual ~C_DynamicSceneNode();
	};

	struct S_Material
	{
		ae::base::S_HashID hid;
		S_Material* next;
	};

	class C_Scene
	{
		NOCOPY_CLASS(C_Scene);
		friend class C_SceneSys;
		// *****************************************************************************************************************
		C_Scene();
		~C_Scene();
		// *****************************************************************************************************************
		typedef std::deque<S_SceneNode*> T_SceneNodes;
		typedef std::deque<C_DynamicSceneNode*> T_DynamicSceneNodes;
		T_SceneNodes m_SceneNodes;
		T_DynamicSceneNodes m_DynamicSceneNodes;
	public:
		class I_NodesEnumerator {
			public :
				virtual void OnNode(S_SceneNode&, S_ScenePrefabBase& ) {};
				virtual void OnDynamicNode(C_DynamicSceneNode&) {};
		};
		// *****************************************************************************************************************
		void AddSceneNode(S_SceneNode*);
		void RemoveSceneNode(S_SceneNode*);
		// *****************************************************************************************************************
		C_DynamicSceneNode* CreateDynamicNode(const char* meshResource, const ae::math::float3& pos);
		void DestroyDynamicNode(C_DynamicSceneNode*);
		// *****************************************************************************************************************
		C_DirectionalLight* CreateDirectionalLight();
		C_PointLight* CreatePointLight();
		void DestroyDirectionalLight(C_DirectionalLight*);
		void DestroyPointLight(C_PointLight*);
		// *****************************************************************************************************************
		void EnumerateNodes(I_NodesEnumerator&, E_ScenePrefab_Type);
		void EnumerateDynamicNodes(I_NodesEnumerator&);
		// *****************************************************************************************************************
#ifdef AE_EDITOR
		void UpdateNodeTransformation(const char* nodeName, const ae::math::float4x4& transformation);
		void UpdateGeometryInstanceTransformations(const char* prefabName, const char* geometryName, const ae::math::float4x4* transformations, ae::u32 numTransformations);
#endif
	private:
		void EnumerateNodes(S_SceneNode&, S_ScenePrefabBase&, E_ScenePrefab_Type, I_NodesEnumerator&);
	};
} } // namespace ae { namespace scene {

#endif // #ifdef AE_SCENE_SYS_SCENE_H
