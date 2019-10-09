#pragma once

#ifndef AE_PACKAGES_MANAGER_H
#define AE_PACKAGES_MANAGER_H

#include "../../base_sys/file_system/file_system.h"
#include "../../base_sys/xml/xml_editable.h"
#include "../resource.h"

namespace ae { namespace base {
	struct S_XMLNode;
} } // namespace ae { namespace base {

namespace ae { namespace resource {
	struct S_Init;
// *****************************************************************************************************
// *****************************************************************************************************
	class C_PackagesManager
	{
		C_PackagesManager();
		NOCOPY_CLASS(C_PackagesManager);
		friend class C_ResourceSys;
// *****************************************************************************************************
		C_PackagesManager(const S_Init&);
		~C_PackagesManager();
// *****************************************************************************************************
		std::string m_PathPackageSources;
		std::string m_PathPackedPackages;
		std::string m_PathTmpResources;
		std::string PackageSourcePath(const char* packageSrcName);
// *****************************************************************************************************
		struct S_PackResource
		{
			std::string		sourceFile;		// path to source file
			std::string		resourceFile;	// path to tmp resources
			std::string		inPackageName;	// path inside package

			T_ResourceType	rscType;
			size_t			resourceFileSize;

			ae::base::S_XMLNode* nodeInfo; // Holds attributes for packed resource

			S_PackResource() {}

			S_PackResource(const char* rscType, const char* srcFile, const char* rscFile, const char* inPckgName, ae::base::S_XMLNode* node) : 
				rscType(String2ResourceType(rscType)), 
				sourceFile(srcFile), resourceFile(rscFile),
				inPackageName(inPckgName),
				resourceFileSize(0),
				nodeInfo(node)
				{}
		};
		typedef std::deque<S_PackResource> T_PackResources;

// *****************************************************************************************************
		// return calid S_PackResource struct that contains all information to import and pack resource
		S_PackResource GetPackResource(ae::base::S_XMLNode&, const std::string& path, const char* packageName);

		// Collect specific resource given by rscNode
		bool CollectResourceToPack(ae::base::S_XMLNode&, ae::base::S_XMLNode& rscNode, const std::string& path, const char* packageName,S_PackResource& packResource);

		// Collect all resources from package source
		void CollectResourcesToPack(ae::base::S_XMLNode&, const std::string& path, const char* packageName, T_PackResources& resourcesToPack);

		// Compare source and resource file modification time, if is diferrent return true and resource must be reimported
		bool NeedReimport(S_PackResource& packResource);

		// If NeedReimport == true resource is reimported
		void ReimportResource(S_PackResource& packResource);

		// For every resource inside resourcesToPack if NeedReimport == true resource is reimported
		void ReimportResources(T_PackResources& resourcesToPack);

		// return sum of file sizes of each resource file
		size_t SkipInvalidFilesAndSumSizes(T_PackResources& resourcesToPack);

		// Create package
		void CreatePackage(T_PackResources& resourcesToPack, const char* packageName);
	public:
// *****************************************************************************************************
#ifdef AE_EDITOR
		class C_PackageSourcesEnumerator : public ae::base::I_FileEnumerator
		{
			virtual void OnFile(const char* fileName, const char* path);
		public:
			virtual ~C_PackageSourcesEnumerator() {}
			virtual void OnPackage(const char* packageSrcName) = 0;
		};

		std::string SaveTmpResource(const char* packageName, ae::base::C_XML_Editable<>&, ae::base::S_XMLNode& srcNode); 
		void SavePackageSource(const char* packageSrcName, ae::base::C_XML_Editable<>&);
		void DeletePackageSource(const char* packageSrcName);
#endif // #ifdef AE_EDITOR
// *****************************************************************************************************
		void EnumeratePackageSources(C_PackageSourcesEnumerator&);
		void LoadPackageSource(const char* packageSrcName, ae::base::C_XML_Editable<>&);
		bool IsPackageSource(const char* packageSrcName);
		void PackPackageResource(const char* packageSrcName);
	};

// *****************************************************************************************************
// *****************************************************************************************************
} } // namespace ae { namespace resource {

#endif // #ifdef AE_PACKAGES_MANAGER_H
