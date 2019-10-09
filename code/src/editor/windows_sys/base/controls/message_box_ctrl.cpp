#include "stdafx.h"

#if defined(AE_EDITOR)

#include "message_box_ctrl.h"

#include "../../base/window.h"

#if (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#include "../win32/controls/message_box_ctrl_win32.h"
#elif
	#error No window platform implemented !
#endif

namespace ae { namespace windows_sys {

T_MessageBoxReturnCode C_MessageBoxCtrl::Show(C_Window* pParent, T_MessageBoxStyle style, const char* sTitle, const char* sText)
{
	return T_MessageBoxCtrl_Impl::Show(pParent, style, sTitle, sText);
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
