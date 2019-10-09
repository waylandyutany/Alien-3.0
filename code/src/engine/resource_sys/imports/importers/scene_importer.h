#pragma once

#ifndef AE_RESOURCE_SYS_SCENE_IMPORTER_H
#define AE_RESOURCE_SYS_SCENE_IMPORTER_H

#include "../imports.h"

namespace ae { namespace scene {
	struct S_SceneNode;
	struct S_SceneResource;
} } // namespace ae { namespace scene {

namespace ae { namespace base {
	struct S_XMLNode;
	class C_IncrementalAllocator;
} } // namespace ae { namespace base {

namespace ae { namespace resource {

	class C_SceneImporter : public I_Importer
	{
		typedef C_Import<ae::scene::S_SceneResource> T_Import;
		size_t CalculateSizeOfNodes(const ae::base::S_XMLNode& node) const;
		void ImportNode(ae::scene::S_SceneNode**, const ae::base::S_XMLNode&, T_Import& import);
		void ImportPointers(ae::scene::S_SceneNode*, T_Import& import);
	public:
		bool Import(const char* fileToImport, const char* resourceToExport, const ae::base::S_XMLNode*);
	};

} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_SCENE_IMPORTER_H
