#include "StdAfx.h"

#include "resources_manager.h"

#include "../resource_sys.h"
#include "../package.h" 

#include "../../base_sys/file_system/file_system.h"
#include "../../debug_sys/output/outputs.h"

namespace ae { namespace resource {

// *********************************************************************************************************
C_ResourcesManager::C_ResourcesManager(const S_Init& sInit) :
	m_PathPackedPackages(sInit.sPackedPackagesPath)
{
}

C_ResourcesManager::~C_ResourcesManager()
{
	AE_ASSERT(m_Handlers.empty() && "Not all resource handlers unregistered !");
}

// *********************************************************************************************************
void C_ResourcesManager::RegisterPackageNotifier(I_PackageNotify* pNotifier)
{
	AE_ASSERT(std::find(m_PackageNotifiers.begin(), m_PackageNotifiers.end(), pNotifier) == m_PackageNotifiers.end() && "Package notifier already registered !");
	m_PackageNotifiers.push_back(pNotifier);
}

void C_ResourcesManager::UnregisterPackageNotifier(I_PackageNotify* pNotifier)
{
	T_PackageNotifiers::iterator i = std::find(m_PackageNotifiers.begin(), m_PackageNotifiers.end(), pNotifier);
	AE_ASSERT(i != m_PackageNotifiers.end() && "Trying to unregister not registered notifier !");
	m_PackageNotifiers.erase(i);
}
// *********************************************************************************************************
void C_ResourcesManager::RegisterHandler(T_ResourceType resourceTypeID, I_ResourceHandler* pResourceHandler)
{
	AE_ASSERT(m_Handlers.find(resourceTypeID) == m_Handlers.end() && "Resource handler for given type already registered !");
	m_Handlers.insert(T_ResourceHandlerPair(resourceTypeID, pResourceHandler));
}

void C_ResourcesManager::UnregisterHandler(T_ResourceType resourceTypeID, I_ResourceHandler* pResourceHandler)
{
	T_ResourceHandlerMap::iterator i = m_Handlers.find(resourceTypeID);
	AE_ASSERT(i != m_Handlers.end() && "Unknown resource handler with given type to unregister !");

	if(i->second != pResourceHandler)
	{
		AE_ASSERT(00 && "Resource handler mismatch for given type to unregister !");
	}
	m_Handlers.erase(i);
}

void C_ResourcesManager::PrintDebugInfo(S_Resource* pResource)
{
	DBG_OUT << "*****************************" << std::endl;
	T_ResourceHandlerMap::iterator resourceHandlerIterator = m_Handlers.find(pResource->resourceType);

	if(resourceHandlerIterator != m_Handlers.end())
	{
		resourceHandlerIterator->second->AssignResource(pResource, pResource->Next());
		resourceHandlerIterator->second->PrintDebug();
	}
	else
	{
		DBG_ERR << "No resource handler for given type of resource registered !" << std::endl;
	}
}

void C_ResourcesManager::PrintDebugInfo(void* pMemory, ae::u32 memorySize)
{
	DBG_OUT << "Resources debug info :" << std::endl;
	S_Resource* pResource = static_cast<S_Resource*>(pMemory);
	S_Resource* pResourceEnd = reinterpret_cast<S_Resource*>(static_cast<char*>(pMemory) + memorySize);

	while(pResource < pResourceEnd)
	{
		PrintDebugInfo(pResource);
		pResource = reinterpret_cast<S_Resource*>(reinterpret_cast<char*>(pResource) + pResource->totalSize);
	}
}

// *********************************************************************************************************
// *********************************************************************************************************
bool C_ResourcesManager::IsPackageLoaded(const char* packageName) const
{
	const ae::base::T_Hash32 packageHash(ae::base::CalculateHashI32(packageName));
	return m_Packages.find(packageHash) == m_Packages.end() ? false : true;
}

bool C_ResourcesManager::LoadPackage(const char* packageName)
{
// *********************************************************************************************************
	ae::base::T_Hash32 packageHash(ae::base::CalculateHashI32(packageName));
	T_Packages::iterator it(m_Packages.find(packageHash));
	if(it != m_Packages.end()) {
		DBG_ERR << "Package '" << packageName << "' already loaded !" << std::endl;
		return false;
	}
// *********************************************************************************************************
	std::string packagePath = m_PathPackedPackages + "/";
	packagePath += packageName;
	packagePath += ".pckg";

	ae::base::C_FileSys& fileSys(ae::base::C_FileSys::Inst());
	const size_t packageFileSize(fileSys.GetFileSize(packagePath.c_str()));
	if(!packageFileSize) {
		DBG_ERR << "Invalid package '" << packageName << "' to load !" << std::endl;
		return false;
	}
// *********************************************************************************************************
	S_Package* pPackage = (S_Package*)::malloc(packageFileSize);
	if(fileSys.ReadFile(packagePath.c_str(), (char*)pPackage, packageFileSize) != packageFileSize) {
		DBG_ERR << "Error during package '" << packageName << "' load !" << std::endl;
		::free(pPackage);
		return false;
	}
// *********************************************************************************************************
	m_Packages[packageHash] = pPackage;

	ActivatePackage(pPackage);
// *********************************************************************************************************
	std::for_each(m_PackageNotifiers.begin(),m_PackageNotifiers.end(),[packageName](I_PackageNotify* pNotify) { pNotify->OnPackageLoaded(packageName); });
// *********************************************************************************************************
	return true;
}

bool C_ResourcesManager::UnloadPackage(const char* packageName)
{
// *********************************************************************************************************
	std::for_each(m_PackageNotifiers.begin(),m_PackageNotifiers.end(),[packageName](I_PackageNotify* pNotify) { pNotify->OnPackageUnloading(packageName); });
// *********************************************************************************************************
	ae::base::T_Hash32 packageHash(ae::base::CalculateHashI32(packageName));
	T_Packages::iterator it(m_Packages.find(packageHash));
	if(it == m_Packages.end()) {
		DBG_ERR << "Package '" << packageName << "' already unloaded !" << std::endl;
		return false;
	}
// *********************************************************************************************************
	DeactivatePackage(it->second);
// *********************************************************************************************************
	::free(it->second);
	m_Packages.erase(it);
// *********************************************************************************************************
	return true;
}

// *********************************************************************************************************
void C_ResourcesManager::ActivatePackage(S_Package* pPackage)
{
// *********************************************************************************************************
//	Resolving pointers
	pPackage->resourceChunks = (S_Package::S_ResourceChunk*)((char*)pPackage + sizeof(S_Package));
	for(ae::u32 i = 0; i < pPackage->numResourceChunks; i++)
	{
		S_Resource* pBeginResource = (S_Resource*)((char*)pPackage + pPackage->resourceChunks[i].resourceOffset);
		S_Resource* pEndResource = (S_Resource*)((char*)pBeginResource + pPackage->resourceChunks[i].resourceSize);
		ActivateResources(pBeginResource, pEndResource);
	}
}

void C_ResourcesManager::DeactivatePackage(S_Package* pPackage)
{
// *********************************************************************************************************
//	Deactivating resources with their registered handlers
	for(ae::u32 i = 0; i < pPackage->numResourceChunks; i++)
	{
		S_Resource* pBeginResource = (S_Resource*)((char*)pPackage + pPackage->resourceChunks[i].resourceOffset);
		S_Resource* pEndResource = (S_Resource*)((char*)pBeginResource + pPackage->resourceChunks[i].resourceSize);
		DeactivateResources(pBeginResource, pEndResource);
	}
}
// *********************************************************************************************************

void C_ResourcesManager::ActivateResources(S_Resource* pBeginResource, S_Resource* pEndResource)
{
	typedef std::deque<S_Resource*> T_Resources;
	T_Resources resourcesToActivate;

	for(S_Resource* pResource(pBeginResource); pResource != pEndResource; pResource = pResource->Next())
		resourcesToActivate.push_back(pResource);

	// activating resources in reverse order
	T_Resources::reverse_iterator it(resourcesToActivate.rbegin());
	for(;it != resourcesToActivate.rend(); it++)
	{
		S_Resource* pResource(*it);

		I_ResourceHandler* pResourceHandler = m_Handlers[pResource->resourceType];
		if(!pResourceHandler) {
			DBG_ERR << "Unable find resource handler for '" << ResourceType2String(pResource->resourceType) << "' resource !"<< std::endl;
			break;
		}
		pResourceHandler->AssignResource(pResource, pEndResource);
		if(!pResource->IsActivated())
			pResourceHandler->Activate();
	}
}

void C_ResourcesManager::DeactivateResources(S_Resource* pBeginResource, S_Resource* pEndResource)
{
	typedef std::deque<S_Resource*> T_Resources;
	T_Resources resourcesToActivate;

	for(S_Resource* pResource(pBeginResource); pResource != pEndResource; pResource = pResource->Next())
	{
		I_ResourceHandler* pResourceHandler = m_Handlers[pResource->resourceType];
		if(!pResourceHandler) {
			DBG_ERR << "Unable find resource handler for '" << ResourceType2String(pResource->resourceType) << "' resource !"<< std::endl;
			break;
		}
		pResourceHandler->AssignResource(pResource, pEndResource);
		if(pResource->IsActivated())
			pResourceHandler->Deactivate();
	}
}

// *********************************************************************************************************
// *********************************************************************************************************
bool C_ResourcesManager::ExtractInfoFromResourcePath(const char* resourcePath,	std::string& packageName, 
								std::string& resourceName, T_ResourceType& resourceType, 
								std::string& elementName, T_ResourceType& elementType) const
{
	const std::string sPath(resourcePath);
	const size_t packageNameSlash(sPath.find_first_of('/'));
	const size_t resourceTypeDot(sPath.find_first_of('.'));
	const size_t elementNameSlash(sPath.find_first_of('/',resourceTypeDot));
	if(!packageNameSlash || packageNameSlash == std::string::npos || resourceTypeDot == std::string::npos)
		return false; // Invalid resource path

	packageName = sPath.substr(0, packageNameSlash);
	if(elementNameSlash != std::string::npos) {// we have element as well
		resourceName = sPath.substr(packageNameSlash, elementNameSlash - packageNameSlash);
		elementName = sPath.substr(elementNameSlash + 1);
		elementType = ae::resource::String2ResourceType(ae::base::GetFileExtension(elementName.c_str()).c_str());
	}
	else resourceName = sPath.substr(packageNameSlash);

	resourceType = ae::resource::String2ResourceType(ae::base::GetFileExtension(resourceName.c_str()).c_str());

	return true;
}

// *********************************************************************************************************

S_Resource* C_ResourcesManager::FindResource(const char* resourcePath)
{
	std::string packageName, resourceName, elementName;
	T_ResourceType resourceType, elementType;
	ae::base::T_Hash32 packageHash, resourceHash;
	if(!ExtractInfoFromResourcePath(resourcePath, packageName, resourceName, resourceType, elementName, elementType)) {
		DBG_ERR << "Unable to find resource '" << resourcePath << "'. Invalid resource path !" << std::endl;
		return 0;
	}
	packageHash = ae::base::CalculateHashI32(packageName.c_str());
	resourceHash = ae::base::CalculateHashI32(resourceName.c_str());
// *********************************************************************************************************
	T_Packages::const_iterator it = m_Packages.find(packageHash);
	if(it == m_Packages.end()) {
		DBG_ERR << "Unable to find resource '" << resourcePath << "'. Package doesn't exist !" << std::endl;
		return 0;
	}
// *********************************************************************************************************
	S_Package* pPackage(it->second);
	for(ae::u32 i = 0; i < pPackage->numResourceChunks; i++)
		if(pPackage->resourceChunks[i].resourcePathHash == resourceHash)
		{
			S_Resource* pResource = (S_Resource*)((char*)pPackage + pPackage->resourceChunks[i].resourceOffset);
			if(!pResource->IsActivated())
				ActivateResources(pResource, pResource->Next());
			return pResource;
		}

// *********************************************************************************************************
	DBG_ERR << "Unable to find resource '" << resourcePath << "'. Resource doesn't exist inside package !" << std::endl;
	return 0;
}

// *********************************************************************************************************
// *********************************************************************************************************
void* C_ResourcesManager::FindElement(const char* elementPath)
{
	S_Resource* pResource(FindResource(elementPath));
	if(!pResource) {
		DBG_ERR << "Unable to find element '" << elementPath << "'. Resource doesn't exist inside package !" << std::endl;
		return 0;
	}
// *********************************************************************************************************
	std::string packageName, resourceName, elementName;
	T_ResourceType resourceType, elementType;
	if(!ExtractInfoFromResourcePath(elementPath, packageName, resourceName, resourceType, elementName, elementType)) {
		DBG_ERR << "Unable to find element '" << elementPath << "'. Invalid element path !" << std::endl;
		return 0;
	}
// *********************************************************************************************************
	void* pRet = 0;
	I_ResourceHandler* pResourceHandler = m_Handlers[pResource->resourceType];
	if(pResourceHandler) {
		pResourceHandler->AssignResource(pResource, pResource->Next());
		pRet = pResourceHandler->FindElement(elementName.c_str());
	}
	if(!pRet) {
		DBG_ERR << "Unable to find element '" << elementPath << "'. Element doesn't exist within given resource !" << std::endl;
	}

	return pRet;
}

// *********************************************************************************************************
// *********************************************************************************************************
#ifdef AE_EDITOR
S_Resource* C_ResourcesManager::LoadResource(const char* resourcePath)
{
// *********************************************************************************************************
	ae::base::T_Hash32 resourceHash(ae::base::CalculateHashI32(resourcePath));
	T_Resources::iterator it(m_Resources.find(resourceHash));
	if(it != m_Resources.end()) {
		DBG_ERR << "Resource '" << resourcePath << "' already loaded !" << std::endl;
		return 0;
	}
// *********************************************************************************************************
	ae::base::C_FileSys& fileSys(ae::base::C_FileSys::Inst());
	const size_t resourceFileSize(fileSys.GetFileSize(resourcePath));
	if(!resourceFileSize) {
		DBG_ERR << "Invalid resource '" << resourcePath << "' to load !" << std::endl;
		return 0;
	}
// *********************************************************************************************************
	S_Resource* pResource = (S_Resource*)::malloc(resourceFileSize);
	if(fileSys.ReadFile(resourcePath, (char*)pResource, resourceFileSize) != resourceFileSize) {
		DBG_ERR << "Error during resource '" << resourcePath << "' load !" << std::endl;
		::free(pResource);
		return 0;
	}
// *********************************************************************************************************
	m_Resources[resourceHash] = pResource;
// *********************************************************************************************************
	return pResource;
}

bool C_ResourcesManager::UnloadResource(S_Resource* pResource)
{
// *********************************************************************************************************
	T_Resources::iterator it(m_Resources.begin());
	for(; it != m_Resources.end();++it)
		if(it->second == pResource)
			break;
// *********************************************************************************************************
	if(it == m_Resources.end())
	{
		DBG_ERR << "Given resource to unload doesn't exist !" << std::endl;
		return false;
	}
// *********************************************************************************************************
	::free(pResource);
	m_Resources.erase(it);
// *********************************************************************************************************
	return true;
}

void C_ResourcesManager::EnumerateResourceElements(S_Resource* pResource, I_ResourceElementsEnumerator& enumerator)
{
	if(pResource) {
		I_ResourceHandler* pResourceHandler = m_Handlers[pResource->resourceType];
		if(pResourceHandler) {
			pResourceHandler->AssignResource(pResource, pResource->Next());
			pResourceHandler->EnumerateElements(enumerator);
		}
	}
}

#endif // #ifdef AE_EDITOR
// *********************************************************************************************************
// *********************************************************************************************************

} } // namespace ae { namespace resource {
