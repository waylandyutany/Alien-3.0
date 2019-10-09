#include "StdAfx.h"

#include "render_window_callback.h"
#include "render_device/render_device.h"
#include "render_device/render_window.h"
#include "render_scene/render_scene.h"

#include "../input_sys/input_sys.h"

namespace ae { namespace render {

C_RenderWindowCallback::C_RenderWindowCallback(C_RenderScene& renderScene, C_RenderDevice& renderDevice, C_RenderWindow& renderWindow) :
	m_RenderScene(renderScene), 
	m_RenderDevice(renderDevice),
	m_RenderWindow(renderWindow),
	m_bResizing(false)
{
}

void C_RenderWindowCallback::OnResize(ae::u32 windowWidth, ae::u32 windowHeight)
{
	m_ResizedWindowWidth = windowWidth;
	m_ResizedWindowHeight = windowHeight;
	if(!m_bResizing) OnResizeEnd();
}

void C_RenderWindowCallback::OnFocusLost()
{
	m_RenderWindow.HideAndLockCursor(false);
	ae::input::C_InputSys::Inst().Freeze(true);
}

void C_RenderWindowCallback::OnFocusSet()
{
}

void C_RenderWindowCallback::OnResizeStart()
{
	m_bResizing = true;
}

void C_RenderWindowCallback::OnResizeEnd()
{
	m_RenderScene.SetScreenDimensions(m_ResizedWindowWidth, m_ResizedWindowHeight);

	m_bResizing = false;
}

void C_RenderWindowCallback::OnMouseClick()
{
	if(!m_RenderWindow.IsCursorHiddenAndLocked())
	{
		m_RenderWindow.HideAndLockCursor(true);
		ae::input::C_InputSys::Inst().Freeze(false);
	}
}

} } // namespace ae { namespace render {
