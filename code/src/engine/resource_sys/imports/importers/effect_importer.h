#pragma once

#ifndef AE_RESOURCE_SYS_EFFECT_IMPORTER_H
#define AE_RESOURCE_SYS_EFFECT_IMPORTER_H

#include "../imports.h"

namespace ae { namespace base {
	class C_IncrementalAllocator;
} } // namespace ae { namespace base {

namespace ae { namespace resource {
	namespace shader {
		struct S_CompiledShader;
	}// namespace shader

	class C_EffectImporter : public I_Importer
	{
		size_t CompileShader(char* buff, size_t size, const char* profile, const char* funcName, const char* fileName, shader::S_CompiledShader&, bool& bErrorReported);
	public:
		bool Import(const char* fileToImport, const char* resourceToExport, const ae::base::S_XMLNode*);
	};

} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_EFFECT_IMPORTER_H
