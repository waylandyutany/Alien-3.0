#include "StdAfx.h"

#include "prefabs_importer.h"
#include "import_utils.h"

#include "../../../scene_sys/scene_prefabs/scene_prefabs_resource.h"

#include "../../../base_sys/file_system/file_system.h"
#include "../../../base_sys/memory/allocators/incremental_allocator.h"
#include "../../../debug_sys/output/outputs.h"
#include "../../../_sys/templates.h"

#include "../../../base_sys/xml/xml.h"
#include "../../../base_sys/xml/xml_text_reader.h"

namespace ae { namespace resource {

size_t C_SceneInstanciesImporter::CalculateSizeOfNodes(const ae::base::S_XMLNode& node) const 
{
	size_t sizeSum = 0;
	if(node.Is("prefab")) sizeSum += sizeof(ae::scene::S_ScenePrefab);
	else if(node.Is("lod")) sizeSum += sizeof(ae::scene::S_ScenePrefab_LOD);
	else if(node.Is("rsc"))	{
		if(ae::resource::ResourceType2String(elementTypeMGeo) == node.Attribute("type").AsString())
			sizeSum += sizeof(ae::scene::S_ScenePrefab_MGeometry);
		else if(ae::resource::ResourceType2String(elementTypeCGeo) == node.Attribute("type").AsString())
			sizeSum += sizeof(ae::scene::S_ScenePrefab_CGeometry);
	}
	else if(node.Is("instance")) sizeSum += sizeof(ae::scene::S_Instance);
	
	ae::base::S_XMLNode* pNode = node.child;
	while(pNode)
	{
		sizeSum += CalculateSizeOfNodes(*pNode);
		pNode = pNode->next;
	}
	return sizeSum;
}

void C_SceneInstanciesImporter::ImportTInstancies(ae::u32& numInstancies, ae::scene::S_Instance*& instancies, const ae::base::S_XMLNode& xmlNode, T_Import& import)
{
	numInstancies = 0;
	instancies = 0;
	// Counting number of transformation instancies
	ae::base::S_XMLNode* pNode(xmlNode.child);
	while(pNode)
	{
		if(pNode->Is("instance"))
			numInstancies++;
		pNode = pNode->next;
	}
	if(!numInstancies)
		return;

	// allocating buffer for t instancies
	instancies = (ae::scene::S_Instance*)import.allocator.Alloc(numInstancies * sizeof(ae::scene::S_Instance));
	// copying t instancies matrices to preallocated buffer
	pNode = xmlNode.child;
	numInstancies = 0;
	while(pNode)
	{
		if(pNode->Is("instance"))
		{
			GetMatrixFromXMLTransformation(*pNode, instancies[numInstancies].worldMatrix);
			instancies[numInstancies].invWorldMatrix = instancies[numInstancies].worldMatrix.Inverse();
			numInstancies++;
		}
		pNode = pNode->next;
	}
}

void C_SceneInstanciesImporter::ImportNode(ae::scene::S_ScenePrefabBase** pSceneInstNode, const ae::base::S_XMLNode& node, T_Import& import)
{
	ae::base::S_XMLNode* pNode = node.child;
	while(pNode)
	{
		if(pNode->Is("prefab"))
		{
			ae::scene::S_ScenePrefab** pNodes = (ae::scene::S_ScenePrefab**)pSceneInstNode;
			ae::scene::S_ScenePrefab* pSceneInstance = ae::CreateStruct<ae::scene::S_ScenePrefab>(*pNodes, import.allocator);
			pSceneInstance->type = ae::scene::E_SINT_Instance;
			pSceneInstance->hid.id = AllocString(import.allocator, pNode->Attribute("name").AsString());
			pSceneInstance->hid.idHash = ae::base::CalculateHashI32(pSceneInstance->hid.id);
			ImportNode(&pSceneInstance->child, *pNode, import);
		}
		else if(pNode->Is("lod"))
		{
			ae::scene::S_ScenePrefab_LOD** pNodes = (ae::scene::S_ScenePrefab_LOD**)pSceneInstNode;
			ae::scene::S_ScenePrefab_LOD* pSceneLOD = ae::CreateStruct<ae::scene::S_ScenePrefab_LOD>(*pNodes, import.allocator);
			pSceneLOD->type = ae::scene::E_SINT_LOD;
			ImportNode(&pSceneLOD->child, *pNode, import);
		}
		else if(pNode->Is("collision"))
		{
			ImportNode(&(*pSceneInstNode), *pNode, import);
		}
		else if(pNode->Is("rsc"))
		{
			std::string type = pNode->Attribute("type").AsString();
			if(ae::resource::ResourceType2String(elementTypeMGeo) == type)
			{
				ae::scene::S_ScenePrefab_MGeometry** pNodes = (ae::scene::S_ScenePrefab_MGeometry**)pSceneInstNode;
				ae::scene::S_ScenePrefab_MGeometry* pSceneMGeo = ae::CreateStruct<ae::scene::S_ScenePrefab_MGeometry>(*pNodes, import.allocator);
				pSceneMGeo->type = ae::scene::E_SINT_MGeometry;
				pSceneMGeo->elementPath = AllocString(import.allocator, pNode->Attribute("path").AsString());
				pSceneMGeo->bCastShadows = pNode->Node("shadows").Attribute("cast").AsUInt(0) ? true : false;
				ImportTInstancies(pSceneMGeo->numInstances, pSceneMGeo->instances, *pNode, import);
			}
			else if(ae::resource::ResourceType2String(elementTypeCGeo) == type)
			{
				ae::scene::S_ScenePrefab_CGeometry** pNodes = (ae::scene::S_ScenePrefab_CGeometry**)pSceneInstNode;
				ae::scene::S_ScenePrefab_CGeometry* pSceneCGeo = ae::CreateStruct<ae::scene::S_ScenePrefab_CGeometry>(*pNodes, import.allocator);
				pSceneCGeo->type = ae::scene::E_SINT_CGeometry;
				pSceneCGeo->elementPath = AllocString(import.allocator, pNode->Attribute("path").AsString());
				ImportTInstancies(pSceneCGeo->numInstances, pSceneCGeo->instances, *pNode, import);
			}
		}
		pNode = pNode->next;
	}
}

void C_SceneInstanciesImporter::ImportPointers(ae::scene::S_ScenePrefabBase* pNode, T_Import& import)
{
	while(pNode)
	{
		switch(pNode->type)
		{
		case ae::scene::E_SINT_CGeometry :
			{
				ae::scene::S_ScenePrefab_CGeometry* pSceneCGeo = (ae::scene::S_ScenePrefab_CGeometry*)pNode;
				import.AddPointer(&pSceneCGeo->elementPath);
				import.AddPointer(&pSceneCGeo->instances);
			}
			break;
		case ae::scene::E_SINT_MGeometry :
			{
				ae::scene::S_ScenePrefab_MGeometry* pSceneMGeo = (ae::scene::S_ScenePrefab_MGeometry*)pNode;
				import.AddPointer(&pSceneMGeo->elementPath);
				import.AddPointer(&pSceneMGeo->instances);
			}
			break;
		case ae::scene::E_SINT_Instance :
			{
				ae::scene::S_ScenePrefab* pSceneInstance = (ae::scene::S_ScenePrefab*)pNode;
				import.AddPointer(&pSceneInstance->hid.id);
			}
			break;
		case ae::scene::E_SINT_LOD :
			{
//				ae::scene::S_SceneInst_LOD* pSceneLOD = (ae::scene::S_SceneInst_LOD*)pNode;
			}
			break;
		}

		import.AddPointer(&pNode->child);
		ImportPointers(pNode->child, import);
		import.AddPointer(&pNode->next);
		pNode = pNode->next;
	}
}

bool C_SceneInstanciesImporter::Import(const char* fileToImport, const char* resourceToExport, const ae::base::S_XMLNode*)
{
	ae::base::C_FileSys& fileSys(ae::base::C_FileSys::Inst());
// *****************************************************************************************************
//	Load prefabs xml
	ae::base::C_XML<> source;
	ae::base::I_InputStream* pInputStream(fileSys.CreateInputStream(fileToImport));
	if(pInputStream) {
		ae::base::C_XML_Text_Reader<> xmlTextReader(*pInputStream, source);
		delete pInputStream;
	} else return false;
// *****************************************************************************************************
	const size_t resourceSize = CalculateSizeOfNodes(source.Root()) + fileSys.GetFileSize(fileToImport) + sizeof(ae::scene::S_ScenePrefabsResource) + 1024;
	auto import(BeginImport<ae::scene::S_ScenePrefabsResource>(resourceToExport, resourceToExport, resourceTypePrefabs, resourceSize));
	ae::scene::S_ScenePrefabsResource* rscSceneInsts = import.pResource;
// *****************************************************************************************************
	ImportNode((ae::scene::S_ScenePrefabBase**)&rscSceneInsts->instancies, source.Root(), import);
	import.AddPointer(&rscSceneInsts->instancies);
	ImportPointers(rscSceneInsts->instancies, import);
// *****************************************************************************************************
	EndImport<ae::scene::S_ScenePrefabsResource>(import);
	return true;
}

} } // namespace ae { namespace resource {
