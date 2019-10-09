#pragma once
#ifndef AE_PHYSIC_SYS_RESOURCES_H
#define AE_PHYSIC_SYS_RESOURCES_H

#ifdef AE_PHYSX
#include "../physx/resources/resources_physx.h"
#endif // #ifdef AE_PHYSX

#include "../../math_sys/types.h"

namespace ae { namespace base {
	class C_IncrementalAllocator;
} } // namespace ae { namespace base {

namespace ae { namespace physic {

	class C_Device;
	class C_Resources;
	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_Mesh
	{
		friend class C_Resources;
		T_MeshImpl m_Impl;
	public:
		T_MeshImpl& Impl() { return m_Impl ;}
	};
	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_Material
	{
		friend class C_Resources;
		T_MaterialImpl m_Impl;
	public:
		T_MaterialImpl& Impl() { return m_Impl ;}
	};
	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_Resources
	{
		NOCOPY_CLASS(C_Resources);
		friend class C_PhysicSys;
		C_Resources(C_Device& device);
		~C_Resources();

		C_Device& m_Device;
		T_ResourcesImpl m_Impl;
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

} } // namespace ae { namespace physic {

#endif // #ifdef AE_PHYSIC_SYS_RESOURCES_H
