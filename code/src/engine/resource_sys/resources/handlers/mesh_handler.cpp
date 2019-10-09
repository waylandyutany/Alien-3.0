#include "StdAfx.h"

#include "mesh_handler.h"

#include "../resources_manager.h"

#include "../../../render_sys/render_device/resources/resource_vb.h"
#include "../../../render_sys/render_device/resources/resource_ib.h"
#include "../../../render_sys/render_device/resources/resource_effect.h"

#include "../../../debug_sys/output/outputs.h"
#include "../../../math_sys/types_dbg.h"

#include "../../../render_sys/render_device/render_resources.h"
#include "../../../render_sys/render_device/resources/resource_mesh.h"
#include "../../../render_sys/render_device/render_resources.h"
#include "../../../render_sys/render_scene/render_scene.h"
#include "../../../render_sys/render_sys.h"

namespace ae { namespace resource {

C_MeshResourceHandler::C_MeshResourceHandler() :
	I_ResourceHandler()
{
}

/*virtual*/ void C_MeshResourceHandler::Activate()
{
	I_ResourceHandler::Activate();
	ResolvePointers();

	ae::render::S_MeshResource* pMeshRsc = GetResource()->GetData<ae::render::S_MeshResource>();
// *********************************************************************************************************
// *********************************************************************************************************
	pMeshRsc->m_pIBResource = static_cast<ae::render::S_IndexBufferResource*>(GetResource()->Next());
	pMeshRsc->m_pVBResource = static_cast<ae::render::S_VertexBufferResource*>(GetResource()->Next()->Next());

	pMeshRsc->m_pEffect = C_ResourceSys::Inst().Resources().FindResource<ae::render::S_EffectResource>("default/effects/solid_dn_3d.effe");

	if(!ae::render::C_RenderSys::Instance().GetResources().GetImpl()->CreateInputLayout(*pMeshRsc->m_pVBResource, *pMeshRsc->m_pEffect, pMeshRsc->m_InputLayoutResource))
		DBG_ERR << "Unable to create Input layout !" << std::endl;
	{
		ae::render::S_MeshResource::S_Material* pMaterial = pMeshRsc->materials;
		ae::render::dx11::T_BufferFlags cbFlags = ae::render::dx11::E_BF_PS;

		// Material shader struct, must be padded to 16 bytes !!!
		struct S_MaterialShaderStruct
		{
			ae::math::float4 diffuseColor;
			ae::u32 flags;
			ae::u32 pd0,pd1,pd2;
		} materialShader;

		while(pMaterial)
		{
			// Texture resource assignment
			if(pMaterial->diffuseTexture.idHash)
			{ // we have texture, lets search for it's resource within this resource
				S_Resource* pResource(GetResource()->Next()->Next()->Next());
				while(pResource != GetEndResource())
				{
					if(pResource->hid.idHash == pMaterial->diffuseTexture.idHash) {
						pMaterial->pDiffuseTexture = (ae::render::S_TextureResource*)pResource;
						break;
					}
						
					pResource = pResource->Next();
				}
			}

			materialShader.diffuseColor = pMaterial->diffuse;
			materialShader.flags = pMaterial->pDiffuseTexture ? 1 : 0;
			if(!ae::render::C_RenderSys::Instance().GetResources().GetImpl()->CreateConstantBufferResource(reinterpret_cast<void*>(&materialShader), sizeof(materialShader), cbFlags, ae::render::dx11::E_BSI_PerMaterial, pMaterial->m_CBuffer))
				DBG_ERR << "Unable to create per material constant buffer !" << std::endl;

			pMaterial = pMaterial->next;
		}
	}

// *********************************************************************************************************
// *********************************************************************************************************

//	PrintDebug();
}

/*virtual*/ void C_MeshResourceHandler::Deactivate()
{
	I_ResourceHandler::Deactivate();

	ae::render::S_MeshResource* pMeshRsc = GetResource()->GetData<ae::render::S_MeshResource>();
	ae::render::C_RenderSys::Instance().GetResources().GetImpl()->DestroyInputLayout(pMeshRsc->m_InputLayoutResource);

	ae::render::S_MeshResource::S_Material* pMaterial = pMeshRsc->materials;
	while(pMaterial)
	{
		ae::render::C_RenderSys::Instance().GetResources().GetImpl()->DestroyConstantBufferResource(pMaterial->m_CBuffer);
		pMaterial = pMaterial->next;
	}
}

/*virtual*/ void C_MeshResourceHandler::EnumerateElements(I_ResourceElementsEnumerator& enumerator)
{
	ResolvePointers();

	ae::render::S_MeshResource* pMeshRsc = GetResource()->GetData<ae::render::S_MeshResource>();
	ae::render::S_MeshResource::S_Geometry* pGeometry = pMeshRsc->geometries;
	while(pGeometry)
	{
		enumerator.OnElement(pGeometry->hid.id, elementTypeMGeo);
		pGeometry = pGeometry->next;
	}
}

/*virtual*/ void* C_MeshResourceHandler::FindElement(const char* elementName)
{
	T_ResourceType resourceType = String2ResourceType(ae::base::GetFileExtension(elementName).c_str());
	if(resourceType != elementTypeMGeo) return 0;

	ae::base::T_Hash32 hashElementName = ae::base::CalculateHashI32(ae::base::GetFileName(elementName, false).c_str());

	ae::render::S_MeshResource* pMeshRsc = GetResource()->GetData<ae::render::S_MeshResource>();
	ae::render::S_MeshResource::S_Geometry* pGeometry = pMeshRsc->geometries;
	while(pGeometry)
	{
		if(pGeometry->hid.idHash == hashElementName)
			return (void*)pGeometry;
		pGeometry = pGeometry->next;
	}

	return 0;
}

void C_MeshResourceHandler::PrintDebug()
{
	I_ResourceHandler::PrintDebug();

	ae::render::S_MeshResource* pMeshRsc = GetResource()->GetData<ae::render::S_MeshResource>();

	DBG_OUT << "Mesh resource" << std::endl;
	DBG_OUT << "Info " << GetResource()->hid.id << std::endl;

// *********************************************************************************************************
	DBG_OUT << "Materials :" << std::endl;
	ae::render::S_MeshResource::S_Material* pMaterial = pMeshRsc->materials;
	while(pMaterial)
	{
		DBG_OUT << "  '" << pMaterial->hid.id << "' diffuse : " << pMaterial->diffuse << std::endl;
		pMaterial = pMaterial->next;
	}

// *********************************************************************************************************
	DBG_OUT << "Geometries :" << std::endl;
	ae::render::S_MeshResource::S_Geometry* pGeometry = pMeshRsc->geometries;
	while(pGeometry)
	{
		DBG_OUT << "  '" << pGeometry->hid.id << "'" << std::endl;
		DBG_OUT << "  Triangles" << std::endl;
		ae::render::S_MeshResource::S_Geometry::S_Triangles* pTriangles = pGeometry->triangles;
		while(pTriangles)
		{
			DBG_OUT << "    Material '" << pTriangles->material->hid.id << "'";
			DBG_OUT << " VB Offset : " << ae::dbg::dec_fmt(4) << pTriangles->vertexOffset << std::endl;
			DBG_OUT << " Index Offset : " << ae::dbg::dec_fmt(4) << pTriangles->indexOffset << " Count : " << ae::dbg::dec_fmt(4) << pTriangles->indexCount << std::endl;
			pTriangles = pTriangles->next;
		}

		pGeometry = pGeometry->next;
	}
}

} } // namespace ae { namespace resource {
