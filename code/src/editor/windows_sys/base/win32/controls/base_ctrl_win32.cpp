#include "stdafx.h"

#if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))

#include "base_ctrl_win32.h"

namespace ae { namespace windows_sys {  namespace win32 {

C_BaseCtrl_Win32::C_BaseCtrl_Win32(C_Window_Win32& control) : m_Control(control)
{
	AE_ASSERT(m_Control.GetParent() && "Each control must have parent !");
	m_Control.GetParent()->AddControl(this);
	m_Control.SetBaseControl(this);
}

/*virtual*/ C_BaseCtrl_Win32::~C_BaseCtrl_Win32()
{
	m_Control.SetBaseControl(0);
	AE_ASSERT(m_Control.GetParent() && "Each control must have parent !");
	m_Control.GetParent()->RemoveControl(this);
}

} } } // namespace ae { namespace windows_sys {  namespace win32 {

#endif // #if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
