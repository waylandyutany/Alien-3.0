#include "StdAfx.h"

#include "render_resources.h"
#include "render_device.h"

namespace ae { namespace render {

// *****************************************************************************************************
// *****************************************************************************************************
S_InstanciesBuffer::S_InstanciesBuffer()
{
	Clear();
}

void S_InstanciesBuffer::Clear()
{
	for(ae::u32 i = 0; i < maxInstancies; i++)
		instanciesMatricies[i] = ae::math::float4x4::identity;
}

// *****************************************************************************************************
// *****************************************************************************************************

C_RenderResources::C_RenderResources(C_RenderDevice* device) :
	m_Impl(new dx11::C_RenderResources(device->GetImpl())),
	m_Device(device)
{
}

C_RenderResources::~C_RenderResources()
{
	delete m_Impl;
}

bool C_RenderResources::ImportTextureResourceToMemory(char* pTextureBuffer, size_t textureBufferSize, ae::base::C_IncrementalAllocator& outputAllocator)
{
	return m_Impl->ImportTextureResourceToMemory(pTextureBuffer, textureBufferSize, outputAllocator);
}

} } // namespace ae { namespace render {
