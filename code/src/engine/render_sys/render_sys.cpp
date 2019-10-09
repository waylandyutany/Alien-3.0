#include "StdAfx.h"

#include "render_sys.h"
#include "render_window_callback.h"

#include "render_device/render_window.h"
#include "render_device/render_device.h"
#include "render_device/render_resources.h"
#include "render_scene/render_scene.h"

#include "../debug_sys/output/outputs.h"

namespace ae { namespace render {

// *****************************************************************************************************
C_RenderSys* C_RenderSys::pInstance = 0;
// *****************************************************************************************************
C_RenderSys::C_RenderSys(S_Init& sInit)
{
	m_pWindow = new C_RenderWindow();
	m_pDevice = new C_RenderDevice(sInit, m_pWindow);
	m_pResources = new C_RenderResources(m_pDevice);
	m_pScene = new C_RenderScene(m_pDevice, m_pResources, sInit);
	m_pRenderWindowCallback = new C_RenderWindowCallback(*m_pScene, *m_pDevice, *m_pWindow);
	m_pWindow->RegisterWindowCallback(m_pRenderWindowCallback);
	m_pWindow->SetWindowDimensions(sInit.windowWidth, sInit.windowHeight);
	m_pWindow->HideAndLockCursor(true);
}

C_RenderSys::~C_RenderSys()
{
	m_pWindow->UnregisterWindowCallback(m_pRenderWindowCallback);

	delete m_pRenderWindowCallback;
	delete m_pScene;
	delete m_pResources;
	delete m_pDevice;
	delete m_pWindow;
}
// *****************************************************************************************************
bool C_RenderSys::IsFullScreenMode() const
{
	return m_pDevice->IsFullScreenMode();
}

void C_RenderSys::GetResolution(ae::u32& screenWidth, ae::u32& screenHeight) const
{
	m_pWindow->GetWindowDimensions(screenWidth, screenHeight);
}

void C_RenderSys::ChangeFullScreenMode(bool fullScreen)
{
	m_pDevice->ChangeFullScreenMode(fullScreen);
}

void C_RenderSys::ChangeResolution(ae::u32 screenWidth, ae::u32 screenHeight, bool fullScreen)
{
	m_pDevice->ChangeResolution(screenWidth, screenHeight, fullScreen);
}
// *****************************************************************************************************

bool C_RenderSys::Create(S_Init& sInit)
{
	AE_ASSERT(!pInstance && "C_RenderSys instance already created !");
	pInstance = new C_RenderSys(sInit);
	return true;
}

void C_RenderSys::Release()
{
	AE_ASSERT(pInstance && "Uninitialized C_RenderSys class instance !");
	delete pInstance;
}

C_RenderSys& C_RenderSys::Instance()
{
	AE_ASSERT(pInstance && "Uninitialized C_RenderSys class instance !");
	return *pInstance;
}
// *****************************************************************************************************
void C_RenderSys::Update()
{
	m_pScene->PreRender();
	m_pScene->Render();
}
// *****************************************************************************************************
} } // namespace ae { namespace render {
