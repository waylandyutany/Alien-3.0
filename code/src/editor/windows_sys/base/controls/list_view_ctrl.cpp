#include "stdafx.h"

#if defined(AE_EDITOR)

#include "list_view_ctrl.h"

namespace ae { namespace windows_sys {

C_ListViewCtrl::C_ListViewCtrl(C_Window& control) : m_Impl(control)
{
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
