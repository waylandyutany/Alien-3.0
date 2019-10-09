#pragma once
#if !defined(AE_LIST_VIEW_CTRL_H) && defined(AE_EDITOR)
#define AE_LIST_VIEW_CTRL_H

#if (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#include "../win32/controls/list_view_ctrl_win32.h"
#elif
	#error No window platform implemented !
#endif

#include "../window.h"

namespace ae { namespace windows_sys {

	class C_ListViewCtrl
	{
		C_ListViewCtrl();
		NOCOPY_CLASS(C_ListViewCtrl);
		T_ListViewCtrl_Impl m_Impl;
	public:
		C_ListViewCtrl(C_Window& control);
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_LIST_VIEW_CTRL_H) && defined(AE_EDITOR)
