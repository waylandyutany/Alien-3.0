#pragma once
#if !defined(AE_BASE_CTRL_WIN32_H) && defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#define AE_BASE_CTRL_WIN32_H

#include "../../win32/window_win32.h"

namespace ae { namespace windows_sys {  namespace win32 {

	// *****************************************************************************************************
	// *****************************************************************************************************
	struct S_WPReturn
	{
		LRESULT returnCode;
		bool defaultWndProcCalled; // true is default window procedure has been called and we don't want to call it again
		S_WPReturn(LRESULT retCode, bool dfltWndProcCalled = false) : returnCode(retCode), defaultWndProcCalled(dfltWndProcCalled) {}
		operator LRESULT() { return returnCode; }
	private:
		S_WPReturn();
	};

	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_BaseCtrl_Win32
	{
		C_BaseCtrl_Win32();
		NOCOPY_CLASS(C_BaseCtrl_Win32);
	protected:
		C_Window_Win32& m_Control;
			
	public:
		C_BaseCtrl_Win32(C_Window_Win32& control);
		virtual ~C_BaseCtrl_Win32();

		virtual S_WPReturn WndProc(UINT umsg, WPARAM wparam, LPARAM lparam, S_WPReturn result) { return result; };

		T_ControlID GetControlID() { return m_Control.GetControlID(); }
	};

} } } // namespace ae { namespace windows_sys {  namespace win32 {

#endif // #if !defined(AE_BASE_CTRL_WIN32_H) && defined(AE_EDITOR)
