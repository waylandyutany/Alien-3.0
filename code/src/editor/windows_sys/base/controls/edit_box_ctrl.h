#pragma once
#if !defined(AE_EDIT_BOX_CTRL_H) && defined(AE_EDITOR)
#define AE_EDIT_BOX_CTRL_H

#if (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#include "../win32/controls/edit_box_ctrl_win32.h"
#elif
	#error No window platform implemented !
#endif

#include "../window.h"

namespace ae { namespace windows_sys {

	class C_EditBoxCtrl
	{
		C_EditBoxCtrl();
		NOCOPY_CLASS(C_EditBoxCtrl);
		T_EditBoxCtrl_Impl m_Impl;
	public:
		C_EditBoxCtrl(C_EditBoxCallback*, C_Window& control);
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_EDIT_BOX_CTRL_H) && defined(AE_EDITOR)
