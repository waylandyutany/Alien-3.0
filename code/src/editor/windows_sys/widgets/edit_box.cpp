#include "stdafx.h"

#if defined(AE_EDITOR)

#include "edit_box.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
C_EditBox::C_EditBox(C_EditBoxCallback* pCallback, C_Widget* pParent, T_ControlID id) :
	C_Widget(pParent, windowType_EditBox, windowStyle_TabStop | windowStyle_Border | windowStyle_Visible | windowStyle_Child, id),
	m_Control(pCallback, *this)
{
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
