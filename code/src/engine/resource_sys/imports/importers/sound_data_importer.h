#pragma once

#ifndef AE_RESOURCE_SYS_SOUND_DATA_IMPORTER_H
#define AE_RESOURCE_SYS_SOUND_DATA_IMPORTER_H

#include "../imports.h"

namespace ae { namespace resource {

	class C_SoundDataImporter : public I_Importer
	{
	public:
		C_SoundDataImporter();
		bool Import(const char* fileToImport, const char* resourceToExport, const ae::base::S_XMLNode*);
	};

} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_SOUND_DATA_IMPORTER_H
