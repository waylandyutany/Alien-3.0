#pragma once
#if !defined(AE_MENU_H) && defined(AE_EDITOR)
#define AE_MENU_H

#if (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#include "win32/menu_win32.h"
#elif
	#error No window platform implemented !
#endif

namespace ae { namespace windows_sys {
	class C_Window;
	class C_Menu
	{
		T_MenuImpl m_Impl;
		C_Menu(T_MenuImpl& impl) : m_Impl(impl) {}
	public:
		C_Menu();
		~C_Menu();

		void AppendMenu(const char* sLabel, T_ControlID);
		C_Menu AppendSubMenu(const char* sLabel);
		void AppendSeparator();
		T_ControlID TrackPopupMenu(C_Window& parent);
		operator T_MenuImpl&() { return m_Impl; }
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_MENU_H) && defined(AE_EDITOR)
