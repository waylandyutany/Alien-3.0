#include "stdafx.h"

#if defined(AE_EDITOR)

#include "menu.h"
#include "window.h"

namespace ae { namespace windows_sys {

C_Menu::C_Menu()
{
}

C_Menu::~C_Menu()
{
}

void C_Menu::AppendMenu(const char* sLabel, T_ControlID controlID)
{
	m_Impl.AppendMenu(sLabel, controlID);
}

C_Menu C_Menu::AppendSubMenu(const char* sLabel)
{
	return C_Menu(m_Impl.AppendSubMenu(sLabel)); 
}

void C_Menu::AppendSeparator()
{
	m_Impl.AppendSeparator();
}

T_ControlID C_Menu::TrackPopupMenu(C_Window& parent)
{
	return m_Impl.TrackPopupMenu(parent); 
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
