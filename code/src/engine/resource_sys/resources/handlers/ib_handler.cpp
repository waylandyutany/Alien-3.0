#include "StdAfx.h"

#include "ib_handler.h"

#include "../../../debug_sys/output/outputs.h"
#include "../../../render_sys/render_device/resources/resource_ib.h"
#include "../../../render_sys/render_device/render_resources.h"
#include "../../../render_sys/render_sys.h"

#include "../../../math_sys/types_dbg.h"

using ae::render::S_IndexBufferResource;

namespace ae { namespace resource {

C_IBResourceLoader::C_IBResourceLoader() : I_ResourceHandler()
{
}

void C_IBResourceLoader::Activate()
{
	I_ResourceHandler::Activate();
	ResolvePointers();

	ae::render::S_IndexBufferResource* pIBResource = static_cast<ae::render::S_IndexBufferResource*>(GetResource());

	if(!ae::render::C_RenderSys::Instance().GetResources().GetImpl()->CreateIndexBufferResource(pIBResource->GetData<char*>(), pIBResource->GetDataSize(), pIBResource->m_IndexBufferType, pIBResource->m_Buffer))
		DBG_ERR << "C_IBResourceLoader::Activate() Failed !" << std::endl;

//	PrintDebug();
}

void C_IBResourceLoader::Deactivate()
{
	I_ResourceHandler::Deactivate();

	ae::render::S_IndexBufferResource* pIBResource = static_cast<ae::render::S_IndexBufferResource*>(GetResource());

	ae::render::C_RenderSys::Instance().GetResources().GetImpl()->DestroyIndexBufferResource(pIBResource->m_Buffer);
}


void C_IBResourceLoader::PrintDebug()
{
	I_ResourceHandler::PrintDebug();

	S_IndexBufferResource* pIBR(static_cast<S_IndexBufferResource*>(GetResource()));

	const size_t bytesPerIndex = pIBR->m_IndexBufferType == C_IndexBufferResource::E_IBT_16Bit ? 2 : 4;
	const size_t numTriangles = ((pIBR->GetDataSize() / bytesPerIndex) / 3);
	const ae::u16* indicies16 = reinterpret_cast<const ae::u16*>((char*)pIBR + pIBR->headerSize);
	const ae::u32* indicies32 = reinterpret_cast<const ae::u32*>((char*)pIBR + pIBR->headerSize);

	DBG_OUT << "Index buffer resource : " << pIBR->hid.id << std::endl;
	DBG_OUT << "Bytes per index : " << bytesPerIndex;
	DBG_OUT << " Indicies count : " << (pIBR->GetDataSize() / bytesPerIndex);
	DBG_OUT << " Triangles count : " << numTriangles << std::endl;

	for(u32 i = 0; i < numTriangles; i++)
	{
		if(pIBR->m_IndexBufferType == C_IndexBufferResource::E_IBT_16Bit)
		{
			DBG_OUT << "  " << ae::dbg::dec_fmt(4) << i << " : ";
			DBG_OUT << ae::dbg::dec_fmt(4) << indicies16[i*3+0] << " ";
			DBG_OUT << ae::dbg::dec_fmt(4) << indicies16[i*3+1] << " ";
			DBG_OUT << ae::dbg::dec_fmt(4) << indicies16[i*3+2] << std::endl;
		}
		else
		{
			DBG_OUT << "  " << ae::dbg::dec_fmt(4) << i << " : ";
			DBG_OUT << ae::dbg::dec_fmt(4) << indicies32[i*3+0] << " ";
			DBG_OUT << ae::dbg::dec_fmt(4) << indicies32[i*3+1] << " ";
			DBG_OUT << ae::dbg::dec_fmt(4) << indicies32[i*3+2] << std::endl;
		}
	}
}


} } // namespace ae { namespace resource {
