#include "StdAfx.h"

#include "render_device.h"
#include "render_window.h"

namespace ae { namespace render {

// *****************************************************************************************************
// *****************************************************************************************************
C_RenderDevice::C_RenderDevice(const S_Init& sInit, C_RenderWindow* window) :
	m_Impl(new dx11::C_RenderDevice(sInit, window->GetImpl())),
	m_Window(window)
{
}

C_RenderDevice::~C_RenderDevice()
{
	delete m_Impl;
}

// *****************************************************************************************************
bool C_RenderDevice::IsFullScreenMode() const
{
	return m_Impl->IsFullScreenMode();
}

void C_RenderDevice::ChangeFullScreenMode(bool fullScreen)
{
	m_Impl->ChangeFullScreenMode(fullScreen);
}

void C_RenderDevice::ChangeResolution(ae::u32 screenWidth, ae::u32 screenHeight, bool fullScreen)
{
	m_Impl->ChangeResolution(screenWidth, screenHeight, fullScreen);
}

// *****************************************************************************************************
// *****************************************************************************************************

void C_RenderDevice::BeginRender()
{
	m_Impl->BeginRender();
}

void C_RenderDevice::BeginSceneRender()
{
	m_Impl->BeginSceneRender();
}

void C_RenderDevice::BeginStencilShadowsRender()
{
	m_Impl->BeginStencilShadowsRender();
}

void C_RenderDevice::BeginShadowMapRender()
{
	m_Impl->BeginShadowMapRender();
}

void C_RenderDevice::BeginLightRender()
{
	m_Impl->BeginLightRender();
}

void C_RenderDevice::BeginDfrComposeRender()
{
	m_Impl->BeginDfrComposeRender();
}

void C_RenderDevice::Begin2DRender()
{
	m_Impl->Begin2DRender();
}

void C_RenderDevice::EndRender()
{
	m_Impl->EndRender();
}

// *****************************************************************************************************
// *****************************************************************************************************
void C_RenderDevice::ResizeBuffers(ae::u32 viewWidth, ae::u32 viewHeight)
{
	m_Impl->ResizeBuffers(viewWidth, viewHeight);
}

} } // namespace ae { namespace render {
