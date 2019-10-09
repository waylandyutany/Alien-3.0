#include "stdafx.h"

#if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))

#include "list_View_ctrl_win32.h"

namespace ae { namespace windows_sys {  namespace win32 {

C_ListViewCtrl_Win32::C_ListViewCtrl_Win32(C_Window_Win32& control) :
	C_BaseCtrl_Win32(control)
{
}

} } } // namespace ae { namespace windows_sys {  namespace win32 {

#endif // #if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
