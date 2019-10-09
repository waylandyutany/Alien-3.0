#pragma once
#if !defined(AE_LIST_VIEW_CTRL_WIN32_H) && defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#define AE_LIST_VIEW_CTRL_WIN32_H

#include "base_ctrl_win32.h"
#include "../../win32/window_win32.h"

namespace ae { namespace windows_sys {  namespace win32 {

	class C_ListViewCtrl_Win32 : public C_BaseCtrl_Win32
	{
		C_ListViewCtrl_Win32();
		NOCOPY_CLASS(C_ListViewCtrl_Win32);
	public:
		C_ListViewCtrl_Win32(C_Window_Win32& control);
	};

} } } // namespace ae { namespace windows_sys {  namespace win32 {

namespace ae { namespace windows_sys {
	typedef ae::windows_sys::win32::C_ListViewCtrl_Win32 T_ListViewCtrl_Impl;
} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_LIST_VIEW_CTRL_WIN32_H) && defined(AE_EDITOR)
