#include "StdAfx.h"

#include "collision_mesh_exporter.h"

#include "../../../../physic_sys/physic_sys.h"
#include "../../../../physic_sys/resources/resource_collision.h"

#include "../../../../scene_sys/scene/scene.h"

#include "../../../../base_sys/memory/allocators/incremental_allocator.h"
#include "../../../../debug_sys/output/outputs.h"
#include "../../../../math_sys/types_dbg.h"
#include "../../../../math_sys/collision/collision.h"

#include "../../../../_sys/templates.h"

using namespace ae::scene;

namespace ae { namespace resource {

// *****************************************************************************************************
// *****************************************************************************************************
/*virtual*/ void C_CollisionMeshExporter::OnExport(const char* resourceName, T_MeshResource& meshRsc, T_INDBResource& indRsc, T_VTXBResource& vtxRsc, C_ColladaMeshImporter& importer)
{
	auto import(BeginImport<S_Resource>(resourceName, resourceName, resourceTypeCollision, 1024 * 1024 * 4));
	auto rsrcCollision = reinterpret_cast<S_CollisionResource*>(import.allocator.Alloc(sizeof(S_CollisionResource)));

	rsrcCollision->bHasConvexMeshes = m_bImportAsConvex;
	// *****************************************************************************************************
	// export scene materials
	C_ColladaMeshImporter::S_Material* iMaterial = importer.m_pMaterials;
	while(iMaterial)
	{
		S_CollisionResource::S_Material* eMaterial = ae::CreateStruct<S_CollisionResource::S_Material>(rsrcCollision->materials, import.allocator);

		eMaterial->hid.id = AllocString(import.allocator, iMaterial->id);
		eMaterial->hid.idHash = ae::base::CalculateHashI32(iMaterial->id);

		iMaterial = iMaterial->next;
	}

	// *****************************************************************************************************
	// export scene geometries and meshes
	C_ColladaMeshImporter::S_Geometry* iGeometry = importer.m_pGeometries;
	while(iGeometry)
	{
		S_CollisionResource::S_Geometry* eGeometry = ae::CreateStruct<S_CollisionResource::S_Geometry>(rsrcCollision->geometries, import.allocator);
		eGeometry->hid.id = AllocString(import.allocator, iGeometry->id);
		eGeometry->hid.idHash = ae::base::CalculateHashI32(iGeometry->id);

		C_ColladaMeshImporter::S_Geometry::S_Triangles* iTriangles = iGeometry->triangles;
		while(iTriangles)
		{
			S_CollisionResource::S_Geometry::S_Mesh* eMesh = ae::CreateStruct<S_CollisionResource::S_Geometry::S_Mesh>(eGeometry->meshes, import.allocator);
			eMesh->material = ae::FindStructHID<S_CollisionResource::S_Material>(iTriangles->material->id, rsrcCollision->materials);

			iTriangles = iTriangles->next;
		}

		iGeometry = iGeometry->next;
	}
	// *****************************************************************************************************
	// bake collision mesh per each geometry mesh
	S_CollisionResource::S_Geometry* eGeometry(rsrcCollision->geometries);
	while(eGeometry)
	{
		ae::render::S_MeshResource::S_Geometry* rGeometry = ae::FindStructHID(eGeometry->hid.id, meshRsc.geometries);

		S_CollisionResource::S_Geometry::S_Mesh* eMesh(eGeometry->meshes);
		ae::render::S_MeshResource::S_Geometry::S_Triangles* pTriangle(rGeometry->triangles);

		while(pTriangle)
		{
			AE_ASSERT(eMesh && "Must not be null !!!");
			// *****************************************************************************************************
			// Bake collision mesh
			if(indRsc.m_IndexBufferType == C_IndexBufferResource::E_IBT_32Bit)
			{
				ae::u32 numI = pTriangle->indexCount;
				ae::u32* pInd32 = &indRsc.GetData<ae::u32>()[pTriangle->indexOffset];

				// calculation vertex count inside vertex buffer according index buffer
				ae::u32 numV = 0;
				for(ae::u32 i = 0; i < numI; i++)
					if(pInd32[i] > numV) numV = pInd32[i] ;
				// we must increment in order to make max value
				numV++;

				ae::u32 strideV = sizeof(ae::math::float3);
				ae::math::float3* verts = (ae::math::float3*)&vtxRsc.GetData<ae::math::float3>()[pTriangle->vertexOffset];

				eMesh->pBakedMeshBuffer = (void*)import.allocator.GetTopPointer();

				if(!ae::physic::C_PhysicSys::Instance().Device().BakeCollisionMesh(verts,numV, strideV, pInd32, numI, import.allocator, rsrcCollision->bHasConvexMeshes))
				{
					eMesh->pBakedMeshBuffer = 0;
					DBG_ERR	<< "Baking collision mesh has failed !" << std::endl;
				} else {
					eMesh->sizeBakedMeshBuffer = import.allocator.GetRelativeUsage(eMesh->pBakedMeshBuffer);
					DBG_OUT << "Collision mesh has been successfully baked : " <<  eMesh->sizeBakedMeshBuffer << std::endl;
				}
			}
			// *****************************************************************************************************
			pTriangle = pTriangle->next;
			eMesh = eMesh->next;
		}

		AE_ASSERT(!pTriangle && !eMesh && "Must be null !!!");

		eGeometry = eGeometry->next;
	}
	// *****************************************************************************************************
	ImportPointers(rsrcCollision, import);
	EndImport<S_Resource>(import);
}

void C_CollisionMeshExporter::ImportPointers(ae::scene::S_CollisionResource* rsrcCollision, T_Import& import)
{
	import.AddPointer(&rsrcCollision->materials);
	ae::scene::S_CollisionResource::S_Material* pMaterial(rsrcCollision->materials);
	while(pMaterial)
	{
		import.AddPointer(&pMaterial->hid.id);
		import.AddPointer(&pMaterial->next);

		pMaterial = pMaterial->next;
	}

	import.AddPointer(&rsrcCollision->geometries);
	ae::scene::S_CollisionResource::S_Geometry* pGeometry(rsrcCollision->geometries);
	while(pGeometry)
	{
		import.AddPointer(&pGeometry->meshes);
		ae::scene::S_CollisionResource::S_Geometry::S_Mesh* pMesh(pGeometry->meshes);
		while(pMesh)
		{
			import.AddPointer(&pMesh->pBakedMeshBuffer);
			import.AddPointer(&pMesh->material);
			import.AddPointer(&pMesh->next);

			pMesh = pMesh->next;
		}

		import.AddPointer(&pGeometry->hid.id);
		import.AddPointer(&pGeometry->next);

		pGeometry = pGeometry->next;
	}
}

// *****************************************************************************************************
// *****************************************************************************************************

} } // namespace ae { namespace resource {
