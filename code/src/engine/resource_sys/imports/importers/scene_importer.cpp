#include "StdAfx.h"

#include "scene_importer.h"
#include "import_utils.h"

#include "../../../scene_sys/scene/scene_resource.h"

#include "../../../base_sys/file_system/file_system.h"
#include "../../../base_sys/memory/allocators/incremental_allocator.h"
#include "../../../debug_sys/output/outputs.h"
#include "../../../_sys/templates.h"

#include "../../../base_sys/xml/xml.h"
#include "../../../base_sys/xml/xml_text_reader.h"

#include "../../../math_sys/types.h"

namespace ae { namespace resource {

size_t C_SceneImporter::CalculateSizeOfNodes(const ae::base::S_XMLNode& node) const 
{
	size_t sizeSum = 0;

	if(node.Is("node")) sizeSum += sizeof(ae::scene::S_SceneNode);

	ae::base::S_XMLNode* pNode = node.child;
	while(pNode)
	{
		sizeSum += CalculateSizeOfNodes(*pNode);
		pNode = pNode->next;
	}
	return sizeSum;
}

void C_SceneImporter::ImportNode(ae::scene::S_SceneNode** pSceneNodes, const ae::base::S_XMLNode& node, T_Import& import)
{
	ae::base::S_XMLNode* pNode = node.child;
	while(pNode)
	{
		if(pNode->Is("node"))
		{
			ae::scene::S_SceneNode** pNodes = (ae::scene::S_SceneNode**)pSceneNodes;
			ae::scene::S_SceneNode* pSceneNode = ae::CreateStruct<ae::scene::S_SceneNode>(*pNodes, import.allocator);
			pSceneNode->nodePath = AllocString(import.allocator, pNode->Attribute("path").AsString());
			pSceneNode->hid.id = AllocString(import.allocator, pNode->Attribute("name").AsString());
			pSceneNode->hid.idHash = ae::base::CalculateHashI32(pSceneNode->hid.id);
// *****************************************************************************************************
			GetMatrixFromXMLTransformation(*pNode, pSceneNode->worldMatrix);
			pSceneNode->invWorldMatrix = pSceneNode->worldMatrix.Inverse();
// *****************************************************************************************************
		}

		pNode = pNode->next;
	}
}

void C_SceneImporter::ImportPointers(ae::scene::S_SceneNode* pSceneNode, T_Import& import)
{
	while(pSceneNode)
	{
		import.AddPointer(&pSceneNode->hid.id);
		import.AddPointer(&pSceneNode->nodePath);
		import.AddPointer(&pSceneNode->next);
		pSceneNode = pSceneNode->next;
	}
}

bool C_SceneImporter::Import(const char* fileToImport, const char* resourceToExport, const ae::base::S_XMLNode*)
{
	ae::base::C_FileSys& fileSys(ae::base::C_FileSys::Inst());
// *****************************************************************************************************
//	Load scene xml
	ae::base::C_XML<> source;
	ae::base::I_InputStream* pInputStream(fileSys.CreateInputStream(fileToImport));
	if(pInputStream) {
		ae::base::C_XML_Text_Reader<> xmlTextReader(*pInputStream, source);
		delete pInputStream;
	} else return false;
// *****************************************************************************************************
	const size_t resourceSize = CalculateSizeOfNodes(source.Root()) + fileSys.GetFileSize(fileToImport) + sizeof(ae::scene::S_SceneResource) + 1024;
	auto import(BeginImport<ae::scene::S_SceneResource>(resourceToExport, resourceToExport, resourceTypeScene, resourceSize));
	ae::scene::S_SceneResource* rscScene = import.pResource;
// *****************************************************************************************************
	ImportNode((ae::scene::S_SceneNode**)&rscScene->pNode, source.Root(), import);
	import.AddPointer(&rscScene->pNode);
	ImportPointers(rscScene->pNode, import);
// *****************************************************************************************************
	EndImport<ae::scene::S_SceneResource>(import);
	return true;
}

} } // namespace ae { namespace resource {
