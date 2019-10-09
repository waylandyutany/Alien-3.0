#include "StdAfx.h"

#include "vb_handler.h"

#include "../../../debug_sys/output/outputs.h"
#include "../../../math_sys/types_dbg.h"

#include "../../../render_sys/render_device/resources/resource_vb.h"
#include "../../../render_sys/render_device/render_resources.h"
#include "../../../render_sys/render_sys.h"

using ae::render::S_VertexBufferResource;

namespace ae { namespace resource {

C_VBResourceHandler::C_VBResourceHandler() :
	I_ResourceHandler()
{
}

void C_VBResourceHandler::Activate()
{
	I_ResourceHandler::Activate();
	ResolvePointers();

	S_VertexBufferResource* pVBResource = static_cast<S_VertexBufferResource*>(GetResource());

	if(!ae::render::C_RenderSys::Instance().GetResources().GetImpl()->CreateVertexBufferResource(pVBResource->GetData<char*>(), pVBResource->GetDataSize(), pVBResource->vertextSize, false, pVBResource->m_Buffer))
		DBG_ERR << "C_VBResourceLoader::Activate() Failed !" << std::endl;

//	PrintDebug();
}

void C_VBResourceHandler::Deactivate()
{
	I_ResourceHandler::Deactivate();

	S_VertexBufferResource* pVBResource = static_cast<S_VertexBufferResource*>(GetResource());

	ae::render::C_RenderSys::Instance().GetResources().GetImpl()->DestroyVertexBufferResource(pVBResource->m_Buffer);
}

void C_VBResourceHandler::PrintDebug()
{
	I_ResourceHandler::PrintDebug();

	S_VertexBufferResource* pVBR = static_cast<S_VertexBufferResource*>(GetResource());
	DBG_OUT << "Vertex buffer resource : " << pVBR->hid.id << std::endl;

	DBG_OUT << "Format";
	DBG_OUT << ((pVBR->vertexFormat & S_VertexBufferResource::VF_POSITION) ? " Position" : "");
	DBG_OUT << ((pVBR->vertexFormat & S_VertexBufferResource::VF_UV0) ? " UV0" : "");
	DBG_OUT << ((pVBR->vertexFormat & S_VertexBufferResource::VF_NORMAL) ? " Normal" : "");
	DBG_OUT << std::endl;

	const S_VertexBufferResource::S_Vertex* verticies = reinterpret_cast<const S_VertexBufferResource::S_Vertex*>((char*)pVBR + pVBR->headerSize);;

	DBG_OUT << "Vertex size " << pVBR->vertextSize << " Vertices count " << pVBR->numVertices << std::endl;
	DBG_OUT << "Vertices " << std::endl;
	const u32 numVertices = pVBR->numVertices;
	for(u32 i = 0; i < numVertices; i++)
		DBG_OUT << "  " << ae::dbg::dec_fmt(4) << i << ": [" << verticies[i].pos << "] [" << verticies[i].uv << "]" << std::endl;
}


} } // namespace ae { namespace resource {
