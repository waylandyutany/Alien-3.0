#include "StdAfx.h"

#include "packages_manager.h"

#include "../package.h"

#include "../resource_sys.h" // for S_Init
#include "../imports/imports.h"

#include "../../base_sys/xml/xml_text_writer.h"
#include "../../base_sys/xml/xml_text_reader.h"
#include "../../base_sys/streams/i_input_stream.h"
#include "../../base_sys/file_system/file_system.h"

#include "../../debug_sys/output/outputs.h"
#include "../../debug_sys/profiler/profiler.h"

#include "../../base_sys/memory/allocators/incremental_allocator.h"

namespace ae { namespace resource {

void GetPackageID(T_PackageID& packageID)
{
	packageID[0] = 'p';
	packageID[1] = 'c';
	packageID[2] = 'k';
	packageID[3] = 'g';
}

// *****************************************************************************************************
// *****************************************************************************************************
/*virtual*/void C_PackagesManager::C_PackageSourcesEnumerator::OnFile(const char* fileName, const char* /*path*/)
{
	OnPackage(ae::base::GetFileName(fileName, false).c_str());
}

// *****************************************************************************************************
// *****************************************************************************************************

C_PackagesManager::C_PackagesManager(const S_Init& sInit) :
	m_PathPackageSources(sInit.s_PackageSourcesPath),
	m_PathPackedPackages(sInit.sPackedPackagesPath),
	m_PathTmpResources(sInit.sTmpResourcesPath)
{
}

C_PackagesManager::~C_PackagesManager()
{
}

// *****************************************************************************************************
// *****************************************************************************************************

std::string C_PackagesManager::PackageSourcePath(const char* packageSrcName)
{
	return (m_PathPackageSources + "/" + packageSrcName + ".xml");
}

#ifdef AE_EDITOR
void C_PackagesManager::SavePackageSource(const char* packageSrcName, ae::base::C_XML_Editable<>& src)
{
	std::ostream* pOstream(ae::base::C_FileSys::Inst().CreateOutputStream(PackageSourcePath(packageSrcName).c_str(), true));
	ae::base::C_XML_Text_Writer xml_save_writer(src, *pOstream);
	delete pOstream;
}

void C_PackagesManager::DeletePackageSource(const char* packageSrcName)
{
	ae::base::C_FileSys::Inst().DeleteFile(PackageSourcePath(packageSrcName).c_str());
}

#endif // #ifdef AE_EDITOR

void C_PackagesManager::EnumeratePackageSources(C_PackageSourcesEnumerator& enumerator)
{
	ae::base::C_FileSys::Inst().EnumFiles(m_PathPackageSources.c_str(),"*.xml", enumerator);
}

void C_PackagesManager::LoadPackageSource(const char* packageSrcName, ae::base::C_XML_Editable<>& src)
{
	ae::base::I_InputStream* pInputStream(ae::base::C_FileSys::Inst().CreateInputStream(PackageSourcePath(packageSrcName).c_str()));
	if(pInputStream)
	{
		ae::base::C_XML_Text_Reader<> xmlTextReader(*pInputStream, src);
		delete pInputStream;
	}
}

bool C_PackagesManager::IsPackageSource(const char* packageSrcName)
{
	return ae::base::C_FileSys::Inst().FileExist(PackageSourcePath(packageSrcName).c_str());
}

// *****************************************************************************************************
C_PackagesManager::S_PackResource C_PackagesManager::GetPackResource(ae::base::S_XMLNode& node, const std::string& path, const char* packageName)
{
	std::string inPackageFile;
	std::string rscFile(m_PathTmpResources);

	rscFile += "/";
	rscFile += packageName;

	inPackageFile += "/";
	inPackageFile += path;
	inPackageFile += node.Attribute("name").AsString();
	inPackageFile += ".";
	inPackageFile += node.Attribute("type").AsString();

	rscFile += inPackageFile;

	return S_PackResource(node.Attribute("type").AsString(), node.Node("path").Attribute("path").AsString(), rscFile.c_str(), inPackageFile.c_str(), &node);
}

bool C_PackagesManager::CollectResourceToPack(ae::base::S_XMLNode& node, ae::base::S_XMLNode& rscNode, const std::string& path, const char* packageName,S_PackResource& packResource)
{
	ae::base::S_XMLNode* pNode = node.child;
	while(pNode)
	{
		if(pNode->Is("dir"))
		{
			std::string p=path;
			p += pNode->Attribute("name").AsString();
			p += "/";
			if(CollectResourceToPack(*pNode, rscNode, p, packageName, packResource))
				return true;
		}
		else if(pNode->Is("src") && pNode == &rscNode)
		{
			packResource = GetPackResource(*pNode, path, packageName);
			return true;
		}

		pNode = pNode->next;
	}

	return false;
}

void C_PackagesManager::CollectResourcesToPack(ae::base::S_XMLNode& node, const std::string& path, const char* packageName, T_PackResources& resourcesToPack)
{
	ae::base::S_XMLNode* pNode = node.child;
	while(pNode)
	{
		if(pNode->Is("dir"))
		{
			std::string p=path;
			p += pNode->Attribute("name").AsString();
			p += "/";
			CollectResourcesToPack(*pNode, p, packageName, resourcesToPack);
		}
		else if(pNode->Is("src"))
			resourcesToPack.push_back(GetPackResource(*pNode, path, packageName));

		pNode = pNode->next;
	}
}

// *****************************************************************************************************

bool C_PackagesManager::NeedReimport(S_PackResource& packResource)
{
	ae::u64  srcLastWrite, rscLastWrite;
	if(!ae::base::C_FileSys::Inst().GetFileTimes(packResource.sourceFile.c_str(),0,&srcLastWrite,0))
		return false;
	if(!ae::base::C_FileSys::Inst().GetFileTimes(packResource.resourceFile.c_str(),0,&rscLastWrite,0))
		return true;
	else if(srcLastWrite != rscLastWrite)
		return true;
	return false;
}

// *****************************************************************************************************

void C_PackagesManager::ReimportResource(S_PackResource& packResource)
{
	if(C_ResourceSys::Inst().Imports().ImportFile(packResource.sourceFile.c_str(), packResource.resourceFile.c_str(), packResource.rscType, packResource.nodeInfo))
	{
		ae::u64 srcLastWrite;
		ae::base::C_FileSys::Inst().GetFileTimes(packResource.sourceFile.c_str(), 0, &srcLastWrite,0);
		ae::base::C_FileSys::Inst().SetFileTimes(packResource.resourceFile.c_str(), 0, &srcLastWrite,0);
	}
}

void C_PackagesManager::ReimportResources(T_PackResources& resourcesToPack)
{
/*
	for(size_t i = 0; i < resourcesToPack.size(); i++)
		if(NeedReimport(resourcesToPack[i]))
			ReimportResource(resourcesToPack[i]);
*/
	const int size((int)resourcesToPack.size());
	Concurrency::parallel_for(0, size, [&](int i) {
		if(NeedReimport(resourcesToPack[i]))
			ReimportResource(resourcesToPack[i]);
	});
}

// *****************************************************************************************************

size_t C_PackagesManager::SkipInvalidFilesAndSumSizes(T_PackResources& resourcesToPack)
{
	size_t totalSize = 0;
	// Collect resource files size and makes it's sum
	for(T_PackResources::iterator i(resourcesToPack.begin()); i != resourcesToPack.end(); ++i)
	{
		(*i).resourceFileSize = ae::base::C_FileSys::Inst().GetFileSize((*i).resourceFile.c_str());
		totalSize += (*i).resourceFileSize;
	}

	// Remove any zero size resource file and log the filename
	resourcesToPack.erase(std::remove_if(resourcesToPack.begin(), resourcesToPack.end(), [] (S_PackResource& pr) -> bool { 
		if(!pr.resourceFileSize)
			DBG_ERR << "Skiping invalid resource file '" << pr.resourceFile << "' !" << std::endl;
		return !pr.resourceFileSize;
	} ), resourcesToPack.end());

	return totalSize;
}

// *****************************************************************************************************
// *****************************************************************************************************

void C_PackagesManager::CreatePackage(T_PackResources& resourcesToPack, const char* packageName)
{
	const size_t totalResourcesSize = SkipInvalidFilesAndSumSizes(resourcesToPack);
	const size_t totalPackageSize = totalResourcesSize + sizeof(S_Package) * resourcesToPack.size() * sizeof(S_Package::S_ResourceChunk);
	std::string packageFileName = m_PathPackedPackages;
	packageFileName += "/";
	packageFileName += packageName;
	packageFileName += ".pckg";
// *****************************************************************************************************
	ae::base::C_FileSys::S_MappedFile* pMappedFile = ae::base::C_FileSys::Inst().CreateMappedFile(packageFileName.c_str(), totalPackageSize, true);
	if(pMappedFile)
	{
		ae::base::C_IncrementalAllocator allocator;
		allocator.Create(pMappedFile->Buffer<char*>(), pMappedFile->Size());
		allocator.Reset(true);
// *****************************************************************************************************
		S_Package* packageHeader = (S_Package*)allocator.Alloc(sizeof(S_Package));
		GetPackageID(packageHeader->packageID);
		ae::C_BuildInfo::GetPlatformID(packageHeader->platformID);
		packageHeader->numResourceChunks = (ae::u32)resourcesToPack.size();
// *****************************************************************************************************
		packageHeader->resourceChunks = (S_Package::S_ResourceChunk*)allocator.Alloc(sizeof(S_Package::S_ResourceChunk) * packageHeader->numResourceChunks);
// *****************************************************************************************************
		for(size_t i = 0; i < resourcesToPack.size(); i++)
		{
			packageHeader->resourceChunks[i].resourceOffset = allocator.GetUsage();
			packageHeader->resourceChunks[i].resourceSize = resourcesToPack[i].resourceFileSize;
			packageHeader->resourceChunks[i].resourcePathHash = ae::base::CalculateHashI32(resourcesToPack[i].inPackageName.c_str());
			ae::base::C_FileSys::Inst().ReadFile(resourcesToPack[i].resourceFile.c_str(), (char*)allocator.Alloc(resourcesToPack[i].resourceFileSize), resourcesToPack[i].resourceFileSize);
		}
// *****************************************************************************************************
		ae::base::C_FileSys::Inst().CloseMappedFile(pMappedFile);
	}
}

// *****************************************************************************************************
// *****************************************************************************************************

void C_PackagesManager::PackPackageResource(const char* packageSrcName)
{
	ae::base::C_XML_Editable<> src;
	T_PackResources resourcesToPack;
	std::string path;

	ae::base::I_InputStream* pInputStream(ae::base::C_FileSys::Inst().CreateInputStream(PackageSourcePath(packageSrcName).c_str()));
	if(pInputStream)
	{
		PROFILE_BLOCK_BEGIN(Packing);
		DBG_OUT << "Packing '" << packageSrcName << "' package. " << std::endl;

		ae::base::C_XML_Text_Reader<> xmlTextReader(*pInputStream, src);

		CollectResourcesToPack(src.Root(), path, packageSrcName, resourcesToPack);
		ReimportResources(resourcesToPack);
		CreatePackage(resourcesToPack, packageSrcName);

		delete pInputStream;

		PROFILE_BLOCK_END(Packing);
		DBG_OUT << "Package '" << packageSrcName << "' succesfully packed in " << Packing.GetSeconds() << "s." << std::endl;
	} else {
		DBG_ERR << "Invalid '" << packageSrcName << "' package !" << std::endl;
	}
}

// *****************************************************************************************************
// *****************************************************************************************************

std::string C_PackagesManager::SaveTmpResource(const char* packageName, ae::base::C_XML_Editable<>& src, ae::base::S_XMLNode& srcNode)
{
	S_PackResource packResource;
	std::string path;

	if(CollectResourceToPack(src.Root(), srcNode, path, packageName, packResource))
	{
		if(NeedReimport(packResource)) ReimportResource(packResource);
		return packResource.resourceFile;
	}

	return "";
}

} } // namespace ae { namespace resource {
