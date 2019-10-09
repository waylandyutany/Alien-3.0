#include "StdAfx.h"

#include "resources.h"
#include "../device/device.h"

#include "../../debug_sys/output/outputs.h"

namespace ae { namespace physic {

C_Resources::C_Resources(C_Device& device) : m_Device(device), m_Impl(device.Impl())
{

}

C_Resources::~C_Resources()
{

}
// *****************************************************************************************************
// *****************************************************************************************************

bool C_Resources::CreateTriangleMesh(void* pCookedMeshData, size_t sizeCookedMeshData, C_Mesh& mesh) {
	return m_Impl.CreateTriangleMesh(pCookedMeshData, sizeCookedMeshData, mesh.m_Impl);
}

void C_Resources::DestroyTriangleMesh(C_Mesh& mesh) {
	m_Impl.DestroyTriangleMesh(mesh.m_Impl);
}

bool C_Resources::CreateConvexMesh(void* pCookedMeshData, size_t sizeCookedMeshData, C_Mesh& mesh) {
	return m_Impl.CreateConvexMesh(pCookedMeshData, sizeCookedMeshData, mesh.m_Impl);
}

void C_Resources::DestroyConvexMesh(C_Mesh& mesh) {
	m_Impl.DestroyConvexMesh(mesh.m_Impl);
}

// *****************************************************************************************************
// *****************************************************************************************************
bool C_Resources::CreateMaterial(C_Material& material)
{
	return m_Impl.CreateMaterial(material.m_Impl);
}

void C_Resources::DestroyMaterial(C_Material& material)
{
	m_Impl.DestroyMaterial(material.m_Impl);
}
// *****************************************************************************************************
// *****************************************************************************************************

} } // namespace ae { namespace physic {
