#include "stdafx.h"

#if defined(AE_EDITOR)

#include "widget.h"

#include "../layouts/layout.h"

namespace ae { namespace windows_sys {

C_Widget::C_Widget(T_WindowStyle widgetStyle) :
	C_Window(widgetStyle),
	m_pLayout(0)
{
}

C_Widget::C_Widget(C_Widget* pParent, T_WindowStyle widgetStyle) :
	C_Window(pParent, widgetStyle),
	m_pLayout(0)
{
}

C_Widget::C_Widget(C_Widget* pParent, T_WindowType type, T_WindowStyle style, T_ControlID id) :
	C_Window(pParent, type, style, id),
	m_pLayout(0)
{
}

C_Widget::~C_Widget()
{
}

/*virtual */void C_Widget::OnSize(ae::u32 width, ae::u32 height)
{
	if(m_pLayout)
	{
		S_Rect layoutRect;
		GetLayoutRect(layoutRect);
		m_pLayout->SetLayoutRect(layoutRect);
	}
}

void C_Widget::SetLayout(C_Layout* pLayout)
{
	m_pLayout = pLayout;
	if(m_pLayout)
	{
		S_Rect layoutRect;
		GetLayoutRect(layoutRect);
		m_pLayout->SetLayoutRect(layoutRect);
	}
}

/*virtual*/ void C_Widget::GetLayoutRect(S_Rect& layoutRect)
{
	// C_Widget must calculate layout rect in respect of it's parent window so we must do child to screen and screen to parent conversion
	S_Rect clientRect;
	S_Point point;

	GetRect(clientRect, true);
	point.x = clientRect.left;
	point.y = clientRect.top;
	if(m_pParent)
	{
		ClientToScreen(point);
		m_pParent->ScreenToClient(point);
	}
	layoutRect = S_Rect(point.x, point.y, point.x + clientRect.Width(), point.y + clientRect.Height());
}

/*virtual*/void C_Widget::SetLayoutRect(const S_Rect& layoutRect)
{
	SetRect(layoutRect);
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
