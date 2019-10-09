#include "StdAfx.h"

#include "texture_importer.h"

#include "../../../base_sys/file_system/file_system.h"
#include "../../../base_sys/memory/allocators/incremental_allocator.h"
#include "../../../debug_sys/output/outputs.h"

#include "../../../render_sys/render_sys.h"
#include "../../../render_sys/render_device/render_resources.h"
#include "../../../render_sys/render_device/resources/resource_texture.h"
#include "../../../_sys/templates.h"

namespace ae { namespace resource {

C_TextureImporter::C_TextureImporter()
{
}

bool C_TextureImporter::Import(const char* fileToImport, const char* resourceToExport, const ae::base::S_XMLNode*)
{
	ae::base::C_FileSys& fileSys(ae::base::C_FileSys::Inst());
	ae::base::C_FileSys::S_MappedFile* pMappedFile = fileSys.OpenMappedFile(fileToImport);
	if(!pMappedFile) return false;
// *****************************************************************************************************
	const size_t resourceSize = 1024 * 1024 * 4; // @todo better memory use
	std::string resourceName = ae::base::GetFileName(fileToImport, false);
	auto import(BeginImport<ae::render::S_TextureResource>(resourceToExport, resourceName.c_str(), resourceTypeTexture, resourceSize));
// *****************************************************************************************************
	if(!ae::render::C_RenderSys::Instance().GetResources().ImportTextureResourceToMemory(pMappedFile->Buffer<char*>(), pMappedFile->Size(), import.allocator))
		DBG_ERR << "Unable to import texture : '" << fileToImport << "' !" << std::endl;
// *****************************************************************************************************
	EndImport<ae::render::S_TextureResource>(import);
	fileSys.CloseMappedFile(pMappedFile);
	return true;
}

} } // namespace ae { namespace resource {
