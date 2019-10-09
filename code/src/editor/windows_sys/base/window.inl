namespace ae { namespace windows_sys {

// *****************************************************************************************************
inline void C_Window::SetMenu(C_Menu& menu)
{
	T_WindowImpl::SetMenu(menu);
}

inline void C_Window::RemoveMenu()
{
	T_WindowImpl::RemoveMenu();
}

// *****************************************************************************************************
inline void C_Window::EnableWindow(bool bEnable)
{
	T_WindowImpl::EnableWindow(bEnable);
}

inline bool C_Window::IsWindowEnabled() const
{
	return T_WindowImpl::IsWindowEnabled();
}
// *****************************************************************************************************

inline void C_Window::ShowWindow(bool bShow)
{
	T_WindowImpl::ShowWindow(bShow);
}

inline bool C_Window::IsVisible() const
{
	return T_WindowImpl::IsVisible();
}
// *****************************************************************************************************

// *****************************************************************************************************
inline void C_Window::SetText(const char* sTitle)
{
	T_WindowImpl::SetText(sTitle);
}

// *****************************************************************************************************
inline void C_Window::GetRect(S_Rect& rect, bool clientRect) const
{
	T_WindowImpl::GetRect(rect, clientRect);
}

inline void C_Window::SetRect(const S_Rect& rect, bool clientRect)
{
	T_WindowImpl::SetRect(rect, clientRect);
}

inline void C_Window::SetSize(ae::u32 width, ae::u32 height)
{
	T_WindowImpl::SetSize(width, height);
}

inline void C_Window::SetPos(ae::u32 x, ae::u32 y)
{
	T_WindowImpl::SetPos(x,y);
}

} } // namespace ae { namespace windows_sys {

