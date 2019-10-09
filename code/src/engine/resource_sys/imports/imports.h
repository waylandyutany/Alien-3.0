#pragma once

#ifndef AE_RESOURCE_SYS_IMPORTS_H
#define AE_RESOURCE_SYS_IMPORTS_H

#include "../resource.h"
#include "export_pointers_table.h"
#include "../../base_sys/memory/allocators/incremental_allocator.h"
#include "../../base_sys/file_system/file_system.h"

namespace ae { namespace base {
	struct S_XMLNode;
} } // namespace ae { namespace base {

namespace ae { namespace resource {

// *****************************************************************************************************
// *****************************************************************************************************
	template<typename T_Resource>
	class C_Import
	{
	public:
		typedef std::deque<char*> T_Pointers;
		T_Pointers pointers;
		T_Resource* pResource;
		ae::base::C_IncrementalAllocator allocator;
		ae::base::C_FileSys::S_MappedFile* pMappedFile;

		template<typename T_Void>
		void AddPointer(T_Void* pointerAddress) { pointers.push_back(reinterpret_cast<char*>(pointerAddress)); }
	};

	template<typename T_Resource>
	C_Import<T_Resource> BeginImport(const char* fileToExport, const char* resourceName, T_ResourceType type, size_t memoryUsage);

	template<typename T_Resource>
	void EndImport(C_Import<T_Resource>&);
// *****************************************************************************************************
// *****************************************************************************************************
	class I_Importer
	{
	public:
		virtual ~I_Importer() {};
		virtual bool Import(const char* fileToImport, const char* resourceToExport, const ae::base::S_XMLNode*) = 0;
	};
// *****************************************************************************************************
// *****************************************************************************************************
	class C_Imports
	{
		C_Imports() {}
		NOCOPY_CLASS(C_Imports);
		friend class C_ResourceSys;
// *****************************************************************************************************
		struct S_Importer
		{
			T_ResourceType rscType;
			I_Importer* pImporter;
			std::string fileExt;
			std::string description;
		};
	public:
		typedef std::vector<S_Importer> T_Importers;
		T_Importers m_Importers;
// *****************************************************************************************************

		void RegisterImporter(T_ResourceType rscType, const char* fileExt, const char* description, I_Importer*);
		void UnregisterImporter(T_ResourceType rscType);
// *****************************************************************************************************
		bool ImportFile(const char* sourceFile, const char* resourceFile, T_ResourceType rscType, const ae::base::S_XMLNode* pInfoNode = 0);
// *****************************************************************************************************
		void FindImporters(const char* fileExt, T_Importers& importers);
		S_Importer* FindImporter(T_ResourceType rscType);
	};
// *****************************************************************************************************
// *****************************************************************************************************
} } // namespace ae { namespace resource {

#include "imports.inl"

#endif // #ifdef AE_RESOURCE_SYS_IMPORTS_H
