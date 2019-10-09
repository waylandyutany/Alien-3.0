#pragma once
#if !defined(AE_MESSAGE_BOX_CTRL_WIN32_H) && defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#define AE_MESSAGE_BOX_CTRL_WIN32_H

#include "../../window_constants.h"

namespace ae { namespace windows_sys {  namespace win32 {

	class C_Window_Win32;

	class C_MessageBoxCtrl_Win32
	{
	public:
		static T_MessageBoxReturnCode Show(C_Window_Win32* pParent, T_MessageBoxStyle style, const char* sTitle, const char* sText);
	};

} } } // namespace ae { namespace windows_sys {  namespace win32 {

namespace ae { namespace windows_sys {
	typedef ae::windows_sys::win32::C_MessageBoxCtrl_Win32 T_MessageBoxCtrl_Impl;
} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_MESSAGE_BOX_CTRL_WIN32_H) && defined(AE_EDITOR)
