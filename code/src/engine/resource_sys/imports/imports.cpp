#include "StdAfx.h"

#include "imports.h"

#include "../../base_sys/file_system/file_system.h"
#include "../../debug_sys/output/outputs.h"

namespace ae { namespace resource {

void C_Imports::RegisterImporter(T_ResourceType rscType, const char* fileExt, const char* description, I_Importer* pImporter)
{
	S_Importer importer;
	importer.pImporter = pImporter;
	importer.fileExt = fileExt;
	importer.rscType = rscType;
	importer.description = description;

	m_Importers.push_back(importer);
}

void C_Imports::UnregisterImporter(T_ResourceType rscType)
{
	m_Importers.erase(std::remove_if(m_Importers.begin(), m_Importers.end(), [rscType](S_Importer& si) { return si.rscType == rscType; } ));
}

void C_Imports::FindImporters(const char* fileExt, T_Importers& importers)
{
	T_Importers::iterator i = m_Importers.begin();
	for(; i != m_Importers.end();++i)
		if(i->fileExt == fileExt)
			importers.push_back((*i));
}

C_Imports::S_Importer* C_Imports::FindImporter(T_ResourceType rscType)
{
	T_Importers::iterator i = m_Importers.begin();
	for(; i != m_Importers.end();++i)
		if(i->rscType == rscType)
			return &(*i);
	return 0;
}

// *****************************************************************************************************
// *****************************************************************************************************

bool C_Imports::ImportFile(const char* sourceFile, const char* resourceFile, T_ResourceType rscType, const ae::base::S_XMLNode* pInfoNode)
{
	ae::base::C_FileSys& fileSys(ae::base::C_FileSys::Inst());
	S_Importer* importer = FindImporter(rscType);
	if(importer->pImporter)
	{
		if(!fileSys.FileExist(sourceFile)) {
			DBG_ERR << "Invalid source file '" << sourceFile << "' to import !" << std::endl;
			return false;
		}

		DBG_OUT << "Importing '" << ResourceType2String(rscType)<< "' from '" << sourceFile << "'." << std::endl;

		return importer->pImporter->Import(sourceFile, resourceFile, pInfoNode);
	}
	return false;
}

} } // namespace ae { namespace resource {
