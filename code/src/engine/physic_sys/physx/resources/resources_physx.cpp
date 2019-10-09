#include "StdAfx.h"

#include "resources_physx.h"
#include "../device/device_physx.h"
#include "../utils_physx.h"
#include "../../../debug_sys/output/outputs.h"
#include "../../../base_sys/memory/allocators/incremental_allocator.h"
#include "../../../_sys/templates.h"

#include "../../../math_sys/types_dbg.h"

namespace ae { namespace physic { namespace physx {

// *****************************************************************************************************
// *****************************************************************************************************
C_Mesh::C_Mesh() : m_pTriangleMesh(0), m_pConvexMesh(0)
{

}

C_Mesh::~C_Mesh()
{
	AE_ASSERT(!m_pTriangleMesh && "Unreleased triangle mesh object !");
	AE_ASSERT(!m_pConvexMesh && "Unreleased convex mesh object !");
}

// *****************************************************************************************************
// *****************************************************************************************************
C_Material::C_Material() : m_pMaterial(0)
{

}

C_Material::~C_Material()
{
	AE_ASSERT(!m_pMaterial && "Unreleased material object !");
}

// *****************************************************************************************************
// *****************************************************************************************************

C_Resources::C_Resources(C_Device& device) : m_Device(device)
{

}

C_Resources::~C_Resources()
{

}

// *****************************************************************************************************
// *****************************************************************************************************
bool C_Resources::CreateTriangleMesh(void* pCookedMeshData, size_t sizeCookedMeshData, C_Mesh& mesh)
{
	if(!sizeCookedMeshData) return false;
	C_InputStream input(pCookedMeshData, sizeCookedMeshData);
	mesh.m_pTriangleMesh = m_Device.Physics().createTriangleMesh(input);
	return mesh.m_pTriangleMesh ? true : false;
}

void C_Resources::DestroyTriangleMesh(C_Mesh& mesh)
{
	SafeRelease(mesh.m_pTriangleMesh);
}

bool C_Resources::CreateConvexMesh(void* pCookedMeshData, size_t sizeCookedMeshData, C_Mesh& mesh)
{
	if(!sizeCookedMeshData) return false;
	C_InputStream input(pCookedMeshData, sizeCookedMeshData);
	mesh.m_pConvexMesh = m_Device.Physics().createConvexMesh(input);
	return mesh.m_pConvexMesh? true : false;
}

void C_Resources::DestroyConvexMesh(C_Mesh& mesh)
{
	SafeRelease(mesh.m_pConvexMesh);
}

// *****************************************************************************************************
// *****************************************************************************************************
bool C_Resources::CreateMaterial(C_Material& material)
{
	material.m_pMaterial = m_Device.Physics().createMaterial(0.5f, 0.5f, 0.1f);
	return true;
}

void C_Resources::DestroyMaterial(C_Material& material)
{
	SafeRelease(material.m_pMaterial);
}
// *****************************************************************************************************
// *****************************************************************************************************

} } } // namespace ae { namespace physic { namespace physx {
