#include "stdafx.h"

#if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))

#include "edit_box_ctrl_win32.h"
#include "../../../widgets/edit_box.h"

namespace ae { namespace windows_sys {  namespace win32 {

C_EditBoxCtrl_Win32::C_EditBoxCtrl_Win32(C_EditBoxCallback* pCallback, C_Window_Win32& control) :
	m_pCallback(pCallback), 
	C_BaseCtrl_Win32(control)
{
}

C_EditBoxCtrl_Win32::~C_EditBoxCtrl_Win32()
{
}

// *****************************************************************************************************
/*virtual*/ S_WPReturn C_EditBoxCtrl_Win32::WndProc(UINT umsg, WPARAM wparam, LPARAM lparam, S_WPReturn result)
{
	if(!m_pCallback) return result;
// *****************************************************************************************************
	if(umsg == WM_COMMAND)
	{
// *****************************************************************************************************
		if(HIWORD(wparam) == EN_CHANGE)
		{
			m_pCallback->OnEditChange(m_Control.GetControlID());
		}
	}
	return result;
}

} } } // namespace ae { namespace windows_sys {  namespace win32 {

#endif // #if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
