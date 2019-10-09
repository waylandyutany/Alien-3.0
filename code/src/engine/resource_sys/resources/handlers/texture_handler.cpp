#include "StdAfx.h"

#include "texture_handler.h"
#include "../resource_handler.h"

#include "../../../debug_sys/output/outputs.h"

#include "../../../render_sys/render_device/resources/resource_texture.h"
#include "../../../render_sys/render_device/render_resources.h"
#include "../../../render_sys/render_sys.h"

namespace ae { namespace resource {

C_TextureResourceLoader::C_TextureResourceLoader() :
	I_ResourceHandler()
{
}

void C_TextureResourceLoader::Activate()
{
	I_ResourceHandler::Activate();

	ae::render::S_TextureResource* pTextureResource = static_cast<ae::render::S_TextureResource*>(GetResource());
	if(!ae::render::C_RenderSys::Instance().GetResources().GetImpl()->CreateTextureResource(pTextureResource->GetData<char*>(), pTextureResource->GetDataSize(), pTextureResource->m_TextureResource))
		DBG_ERR << "C_TextureResourceLoader::Activate() Failed !" << std::endl;

//	PrintDebug()
}

void C_TextureResourceLoader::Deactivate()
{
	I_ResourceHandler::Deactivate();

	ae::render::S_TextureResource* pTextureResource = static_cast<ae::render::S_TextureResource*>(GetResource());
	ae::render::C_RenderSys::Instance().GetResources().GetImpl()->DestroyTextureResource(pTextureResource->m_TextureResource);
}

void C_TextureResourceLoader::PrintDebug()
{
	I_ResourceHandler::PrintDebug();
//	S_TextureResource* pTXTR = static_cast<S_TextureResource*>(GetResource());
	DBG_OUT << "Texture resource" << std::endl;
}

} } // namespace ae { namespace resource {
