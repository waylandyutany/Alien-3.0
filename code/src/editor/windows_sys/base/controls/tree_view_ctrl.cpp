#include "stdafx.h"

#if defined(AE_EDITOR)

#include "tree_view_ctrl.h"

namespace ae { namespace windows_sys {


C_TreeViewCtrl::C_TreeViewCtrl(C_TreeViewCallback* pCallback, C_Window& control) : m_Impl(pCallback, control)
{
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
