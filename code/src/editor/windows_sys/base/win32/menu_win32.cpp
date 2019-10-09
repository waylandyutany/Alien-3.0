#include "stdafx.h"

#if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))

#include "menu_win32.h"
#include "window_win32.h"

namespace ae { namespace windows_sys {  namespace win32 {

C_Menu_Win32::C_Menu_Win32() : m_bDestroyable(true)
{
	m_MenuID = ::CreateMenu();
}

C_Menu_Win32::~C_Menu_Win32()
{
	if(m_bDestroyable)
		DestroyMenu(m_MenuID);
}

void C_Menu_Win32::AppendMenu(const char* sLabel, T_ControlID commandID)
{
	::AppendMenuA(m_MenuID, MF_STRING, commandID, sLabel);
}

C_Menu_Win32 C_Menu_Win32::AppendSubMenu(const char* sLabel)
{
	T_MenuID subMenuID = ::CreatePopupMenu();
	::AppendMenuA(m_MenuID, MF_STRING | MF_POPUP, (UINT_PTR)subMenuID, sLabel);
	return C_Menu_Win32(subMenuID);
}

void C_Menu_Win32::AppendSeparator()
{
	::AppendMenu(m_MenuID, MF_SEPARATOR, 0, 0);
}

T_ControlID C_Menu_Win32::TrackPopupMenu(C_Window_Win32& parent)
{
	POINT mousePos;
	GetCursorPos(&mousePos);

    SetForegroundWindow(parent);
    T_ControlID ret = (T_ControlID)::TrackPopupMenu(m_MenuID, TPM_BOTTOMALIGN | TPM_LEFTALIGN | TPM_RETURNCMD, mousePos.x, mousePos.y, 0, parent, NULL);
	::PostMessage(parent, WM_NULL, 0, 0);

	parent.WndProc(WM_COMMAND, MAKEWPARAM(ret, 0), 0);
	return ret;
}


} } } // namespace ae { namespace windows_sys {  namespace win32 {

#endif // #if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
