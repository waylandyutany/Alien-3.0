#pragma once

#ifndef AE_RESOURCE_SYS_RESOURCES_MANAGER_H
#define AE_RESOURCE_SYS_RESOURCES_MANAGER_H

#include "resource_handler.h"
#include "../../base_sys/hash.h"

namespace ae { namespace resource {

// *********************************************************************************************************
class C_ResourceSys;
struct S_Package;
struct S_Init;
// *********************************************************************************************************
// *********************************************************************************************************
class I_PackageNotify
{
public:
	virtual ~I_PackageNotify() {};
	virtual void OnPackageLoaded(const char* /*packageName*/) {};
	virtual void OnPackageUnloading(const char* /*packageName*/) {};
};
// *********************************************************************************************************
// *********************************************************************************************************
class C_ResourcesManager
{
	friend class C_ResourceSys;
	NOCOPY_CLASS(C_ResourcesManager);
// *********************************************************************************************************
	std::string m_PathPackedPackages;
// *********************************************************************************************************
	C_ResourcesManager(const S_Init&);
	~C_ResourcesManager();
// *********************************************************************************************************
	typedef std::pair<T_ResourceType, I_ResourceHandler*> T_ResourceHandlerPair;
	typedef std::map<T_ResourceType, I_ResourceHandler*> T_ResourceHandlerMap;
	T_ResourceHandlerMap m_Handlers;
// *********************************************************************************************************
	typedef std::pair<ae::base::T_Hash32, S_Package*> T_PackagePair;
	typedef std::map<ae::base::T_Hash32, S_Package*> T_Packages;
	T_Packages m_Packages;
// *********************************************************************************************************
	typedef std::vector<I_PackageNotify*> T_PackageNotifiers;
	T_PackageNotifiers m_PackageNotifiers;
// *********************************************************************************************************
#ifdef AE_EDITOR
	typedef std::pair<ae::base::T_Hash32, S_Resource*> T_ResourcePair;
	typedef std::map<ae::base::T_Hash32, S_Resource*> T_Resources;
	T_Resources m_Resources;
#endif // #ifdef AE_EDITOR
// *********************************************************************************************************
	void PrintDebugInfo(S_Resource* pResource);
	void PrintDebugInfo(void* pMemory, ae::u32 memorySize);
// *********************************************************************************************************
	void ActivateResources(S_Resource*,S_Resource*);
	void DeactivateResources(S_Resource*,S_Resource*);
	void ActivatePackage(S_Package*);
	void DeactivatePackage(S_Package*);
// *********************************************************************************************************
	// resourcepath = "package/resource_path/resource_name.resource_type"
	S_Resource* FindResource(const char* resourcePath);
	// elementPath = "package/resource_path/resource_name.resource_type/element_name.element_type"
	void* FindElement(const char* elementPath);
public:
// *********************************************************************************************************
	// resourcepath = "package/resource_path/resource_name.resource_type"
	// FindResource assure that it returns activated resource if founded resource was not activated it will activate it on this demand
	template<typename T_ResourceType>
	T_ResourceType* FindResource(const char* resourcePath);
	// elementPath = "package/resource_path/resource_name.resource_type/element_name.element_type"
	template<typename T_ElementType>
	T_ElementType* FindElement(const char* elementPath);
// *********************************************************************************************************
	void RegisterHandler(T_ResourceType resourceType, I_ResourceHandler*);
	void UnregisterHandler(T_ResourceType resourceType, I_ResourceHandler*);
// *********************************************************************************************************
	void RegisterPackageNotifier(I_PackageNotify*);
	void UnregisterPackageNotifier(I_PackageNotify*);
// *********************************************************************************************************
	bool LoadPackage(const char* packageName);
	bool UnloadPackage(const char* packageName);
	bool IsPackageLoaded(const char* packageName) const;
// *********************************************************************************************************
#ifdef AE_EDITOR
	S_Resource* LoadResource(const char* resourcePath);
	bool UnloadResource(S_Resource*);
	void EnumerateResourceElements(S_Resource*, I_ResourceElementsEnumerator&);
#endif // #ifdef AE_EDITOR
	bool ExtractInfoFromResourcePath(const char* resourcePath,	std::string& packageName, 
									std::string& resourceName, T_ResourceType& resourceType, 
									std::string& elementName, T_ResourceType& elementType) const;
};

} } // namespace ae { namespace resource {

#include "resources_manager.inl"

#endif // #ifdef AE_RESOURCE_SYS_RESOURCES_MANAGER_H
