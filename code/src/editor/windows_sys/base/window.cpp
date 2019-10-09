#include "stdafx.h"

#if defined(AE_EDITOR)

#include "window.h"
#include "../layouts/layout.h"

namespace ae { namespace windows_sys {

C_Window::C_Window(T_WindowStyle widgetStyle) :
	T_WindowImpl(widgetStyle),
	m_pParent(0)
{
}

C_Window::C_Window(C_Window* pParent, T_WindowStyle widgetStyle) :
	T_WindowImpl(pParent, widgetStyle),
	m_pParent(pParent)
{
}

C_Window::C_Window(C_Window* pParent, T_WindowType type, T_WindowStyle style, T_ControlID id) :
	T_WindowImpl(pParent, type, style, id),
	m_pParent(pParent)
{
}

C_Window::~C_Window()
{
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
