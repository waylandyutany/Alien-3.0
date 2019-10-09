#include "StdAfx.h"

#include "effect_handler.h"

#include "../../../debug_sys/output/outputs.h"
#include "../../../render_sys/render_device/resources/resource_effect.h"
#include "../../../render_sys/render_device/render_resources.h"
#include "../../../render_sys/render_sys.h"

namespace ae { namespace resource {

C_EffectResourceHandler::C_EffectResourceHandler() :
	I_ResourceHandler()
{
}

void C_EffectResourceHandler::Activate()
{
	I_ResourceHandler::Activate();
	ResolvePointers();
	
	ae::render::S_EffectResource* pEffectResource = static_cast<ae::render::S_EffectResource*>(GetResource());
	if(!ae::render::C_RenderSys::Instance().GetResources().GetImpl()->CreateEffectResource(*pEffectResource, pEffectResource->m_Effect))
			DBG_ERR << "C_ShaderResourceLoader::Activate() Failed !" << std::endl;
}

void C_EffectResourceHandler::Deactivate()
{
	I_ResourceHandler::Deactivate();

	ae::render::S_EffectResource* pEffectResource = static_cast<ae::render::S_EffectResource*>(GetResource());
	ae::render::C_RenderSys::Instance().GetResources().GetImpl()->DestroyEffectResource(pEffectResource->m_Effect);
}

void C_EffectResourceHandler::PrintDebug()
{
	I_ResourceHandler::PrintDebug();
}

} } // namespace ae { namespace resource {
