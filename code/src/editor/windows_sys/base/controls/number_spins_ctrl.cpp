#include "stdafx.h"

#if defined(AE_EDITOR)

#include "number_spins_ctrl.h"

namespace ae { namespace windows_sys {

C_NumberSpinCtrl::C_NumberSpinCtrl(E_NumberSpinType type, C_NumberSpinCallback* pCallback, C_Window& control) : m_Impl(type, pCallback, control)
{
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
