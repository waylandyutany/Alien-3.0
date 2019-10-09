#pragma once
#ifndef AE_PHYSIC_SYS_RESOURCES_PHYSX_H
#define AE_PHYSIC_SYS_RESOURCES_PHYSX_H

#include "../../../math_sys/types.h"

namespace ae { namespace physic {
	class C_Resources;
	class C_Mesh;
	class C_Material;
} } // namespace ae { namespace physic {

namespace ae { namespace base {
	class C_IncrementalAllocator;
} } // namespace ae { namespace base {

namespace ae { namespace physic { namespace physx {

	class C_Device;
	class C_Resources;
	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_Mesh
	{
		friend class C_Resources;
		friend class ae::physic::C_Mesh;
		C_Mesh();
		~C_Mesh();
		PxTriangleMesh* m_pTriangleMesh;
		PxConvexMesh* m_pConvexMesh;
	public:
		PxTriangleMesh& TriangleMesh() { AE_ASSERT(m_pTriangleMesh && "Invalid triangle mesh object !"); return *m_pTriangleMesh; }
		PxConvexMesh& ConvexMesh() { AE_ASSERT(m_pConvexMesh && "Invalid convex mesh object !"); return *m_pConvexMesh; }
		bool IsTriangleMesh() const { return m_pTriangleMesh ? true : false; }
		bool IsConvexMesh() const { return m_pConvexMesh ? true : false; }
	};
	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_Material
	{
		friend class C_Resources;
		friend class ae::physic::C_Material;

		C_Material();
		~C_Material();

		 PxMaterial* m_pMaterial;
	public:
		PxMaterial& Material() { return *m_pMaterial; }
	};
	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_Resources
	{
		NOCOPY_CLASS(C_Resources);

		C_Resources(C_Device&);
		~C_Resources();

		friend class ae::physic::C_Resources;
		C_Device& m_Device;
	public:
		bool CreateTriangleMesh(void* pCookedMeshData, size_t sizeCookedMeshData, C_Mesh&);
		void DestroyTriangleMesh(C_Mesh&);
		bool CreateConvexMesh(void* pCookedMeshData, size_t sizeCookedMeshData, C_Mesh&);
		void DestroyConvexMesh(C_Mesh&);

		bool CreateMaterial(C_Material&);
		void DestroyMaterial(C_Material&);
	};
	// *****************************************************************************************************
	// *****************************************************************************************************

} } } // namespace ae { namespace physic { namespace physx {

namespace ae { namespace physic {
	typedef physx::C_Resources T_ResourcesImpl;
	typedef physx::C_Mesh T_MeshImpl;
	typedef physx::C_Material T_MaterialImpl;
} } // namespace ae { namespace physic {

#endif // #ifdef AE_PHYSIC_SYS_RESOURCES_PHYSX_H
