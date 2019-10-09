#include "StdAfx.h"

#include "render_window.h"

namespace ae { namespace render {

C_RenderWindow::C_RenderWindow() :
	m_Impl(new dx11::C_RenderWindow())
{
}

C_RenderWindow::~C_RenderWindow()
{
	delete m_Impl;
}

void C_RenderWindow::RegisterWindowCallback(ae::input::I_WindowCallback* pWindowCallback)
{
	m_Impl->RegisterWindowCallback(pWindowCallback);
}

void C_RenderWindow::UnregisterWindowCallback(ae::input::I_WindowCallback* pWindowCallback)
{
	m_Impl->UnregisterWindowCallback(pWindowCallback);
}

void C_RenderWindow::SetWindowDimensions(ae::u32 windowWidth, ae::u32 windowHeight)
{
	m_Impl->SetWindowDimensions(windowWidth, windowHeight);
}

void C_RenderWindow::GetWindowDimensions(ae::u32& windowWidth, ae::u32& windowHeight) const
{
	m_Impl->GetWindowDimensions(windowWidth, windowHeight);
}

// *****************************************************************************************************
void C_RenderWindow::HideAndLockCursor(bool bHideAndLock)
{
	m_Impl->HideAndLockCursor(bHideAndLock);
}

bool C_RenderWindow::IsCursorHiddenAndLocked() const
{
	return m_Impl->IsCursorHiddenAndLocked();
}
// *****************************************************************************************************

} } // namespace ae { namespace render {
