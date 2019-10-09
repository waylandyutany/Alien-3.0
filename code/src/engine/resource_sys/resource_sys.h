#pragma once

#ifndef AE_RESOURCE_SYS_H
#define AE_RESOURCE_SYS_H

#include "resource.h"
#include "packages/packages_manager.h"
#include "resources/resources_manager.h"
#include "imports/imports.h"

namespace ae { namespace resource {

struct S_Init
{
	std::string s_PackageSourcesPath;
	std::string sTmpResourcesPath;
	std::string sPackedPackagesPath;
	S_Init(const char* pathPackageSources, const char* pathTmpResources, const char* pathPackedPackages) :
		s_PackageSourcesPath(pathPackageSources), sTmpResourcesPath(pathTmpResources), sPackedPackagesPath(pathPackedPackages)
	{
	}
private:
	S_Init();
};

class C_ResourceSys
{
	NOCOPY_CLASS(C_ResourceSys);

	C_ResourceSys(S_Init&);
	~C_ResourceSys();

	static C_ResourceSys* m_pInstance;

	C_ResourcesManager m_ResourceManager;
	C_PackagesManager m_PackagesManager;
	C_Imports m_Imports;
public:
	static void Create(S_Init&);
	static void Release();
	static C_ResourceSys& Inst();

public:
	C_PackagesManager& Packages() { return m_PackagesManager; }
	C_ResourcesManager& Resources() { return m_ResourceManager; }
	C_Imports& Imports() { return m_Imports; }
};

} } // namespace ae { namespace resource {

using ae::resource::C_ResourceSys;

#endif // #ifdef AE_RESOURCE_SYS_H
