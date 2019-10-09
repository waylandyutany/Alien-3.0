#pragma once

#ifndef AE_RESOURCE_SYS_COLLADA_COLLISION_IMPORTER_H
#define AE_RESOURCE_SYS_COLLADA_COLLISION_IMPORTER_H

#include "../imports.h"

namespace ae { namespace base {
	class C_IncrementalAllocator;
} } // namespace ae { namespace base {

namespace ae { namespace resource {

	class C_ColladaCollisionImporter : public I_Importer
	{
	public:
		C_ColladaCollisionImporter();
		bool Import(const char* fileToImport, const char* resourceToExport, const ae::base::S_XMLNode*);
	};

} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_COLLADA_COLLISION_IMPORTER_H
