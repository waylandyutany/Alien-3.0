#include "stdafx.h"

#if defined(AE_EDITOR)

#include "message_box.h"
#include "widget.h"
#include "../base/controls/message_box_ctrl.h"

namespace ae { namespace windows_sys {

T_MessageBoxReturnCode C_MessageBox::Show(C_Widget* pParent, T_MessageBoxStyle style, const char* sTitle, const char* sText)
{
	return C_MessageBoxCtrl::Show(pParent, style, sTitle, sText);
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
