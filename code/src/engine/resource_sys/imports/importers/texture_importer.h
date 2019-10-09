#pragma once

#ifndef AE_RESOURCE_SYS_TEXTURE_IMPORTER_H
#define AE_RESOURCE_SYS_TEXTURE_IMPORTER_H

#include "../imports.h"

namespace ae { namespace resource {

	class C_TextureImporter : public I_Importer
	{
	public:
		C_TextureImporter();
		bool Import(const char* fileToImport, const char* resourceToExport, const ae::base::S_XMLNode*);
	};

} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_TEXTURE_IMPORTER_H
