#include "stdafx.h"

#if defined(AE_EDITOR)

#include "edit_box_ctrl.h"

namespace ae { namespace windows_sys {

C_EditBoxCtrl::C_EditBoxCtrl(C_EditBoxCallback* pCallback, C_Window& control) : m_Impl(pCallback, control)
{
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
