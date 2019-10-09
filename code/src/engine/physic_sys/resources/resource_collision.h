#pragma once

#ifndef AE_SCENE_SYS_RESOURCE_COLLISION_H
#define AE_SCENE_SYS_RESOURCE_COLLISION_H

#include "../../base_sys/hash.h"
#include "resources.h"

namespace ae { namespace scene {

	struct S_CollisionResource
	{
		struct S_Material
		{
			ae::base::S_HashID hid;

			S_Material* next;
		};

		struct S_Geometry
		{
			struct S_Mesh
			{
				S_Material* material;

				void* pBakedMeshBuffer;
				size_t sizeBakedMeshBuffer;
				ae::physic::C_Mesh m_Mesh;

				S_Mesh* next;
			};

			ae::base::S_HashID hid;
			S_Mesh* meshes;

			S_Geometry* next;
		};

		S_Material* materials;
		S_Geometry* geometries;
		bool bHasConvexMeshes;
	};

} } // namespace ae { namespace scene {

#endif // #ifdef AE_SCENE_SYS_RESOURCE_COLLISION_H
