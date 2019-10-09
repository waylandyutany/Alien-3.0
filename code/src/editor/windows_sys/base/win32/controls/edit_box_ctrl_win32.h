#pragma once
#if !defined(AE_EIT_BOX_CTRL_WIN32_H) && defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#define AE_EIT_BOX_CTRL_WIN32_H

#include "base_ctrl_win32.h"
#include "../../win32/window_win32.h"

namespace ae { namespace windows_sys {
	class C_EditBoxCallback;
} } // namespace ae { namespace windows_sys {

namespace ae { namespace windows_sys {  namespace win32 {

	class C_EditBoxCtrl_Win32 : public C_BaseCtrl_Win32
	{
		C_EditBoxCtrl_Win32();
		NOCOPY_CLASS(C_EditBoxCtrl_Win32);
// *****************************************************************************************************
		C_EditBoxCallback* m_pCallback;
	public:
		C_EditBoxCtrl_Win32(C_EditBoxCallback*, C_Window_Win32& control);
		~C_EditBoxCtrl_Win32();
		virtual S_WPReturn WndProc(UINT umsg, WPARAM wparam, LPARAM lparam, S_WPReturn result);
	};

} } } // namespace ae { namespace windows_sys {  namespace win32 {

namespace ae { namespace windows_sys {
	typedef ae::windows_sys::win32::C_EditBoxCtrl_Win32 T_EditBoxCtrl_Impl;
} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_EIT_BOX_CTRL_WIN32_H) && defined(AE_EDITOR)
