#include "stdafx.h"

#if defined(AE_EDITOR)

#include "number_spins.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
C_NumberSpin::C_NumberSpin(E_NumberSpinType type, C_NumberSpinCallback* pCallback, C_Widget* pParent, T_ControlID id) :
	C_Widget(pParent, windowType_NumberEdit, windowStyle_TabStop | windowStyle_Border | windowStyle_Visible | windowStyle_Child, id),
	m_Control(type, pCallback, *this)
{
}

C_FloatSpin::C_FloatSpin(C_NumberSpinCallback* pCallback, C_Widget* pParent, T_ControlID id) :
	C_NumberSpin(E_NST_Float, pCallback, pParent, id)
{
	SetValue(0.0f);
}

C_IntegerSpin::C_IntegerSpin(C_NumberSpinCallback* pCallback, C_Widget* pParent, T_ControlID id) :
	C_NumberSpin(E_NST_Integer, pCallback, pParent, id)
{
	SetValue(0);
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
