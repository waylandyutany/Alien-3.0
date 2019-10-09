#include "stdafx.h"

#if defined(AE_EDITOR)

#include "wdialog.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
C_WDialog::C_WDialog(C_Widget* pParent, const char* sTitle) :
	C_Widget(pParent, windowStyle_Visible | windowStyle_DlgFrame | windowStyle_TiledWindow),
	m_ReturnCode(returnCancel)
{
	SetText(sTitle);
}

/*virtual*/ void C_WDialog::GetLayoutRect(S_Rect& layoutRect)
{
	GetRect(layoutRect, true);
}

/*virtual*/ void C_WDialog::OnClose()
{
	EndDialog(returnCancel);
}

void C_WDialog::EndDialog(T_ReturnCode returnCode)
{
	m_ReturnCode = returnCode;
	EndModal();
}

C_WDialog::T_ReturnCode C_WDialog::DoModal()
{
	C_Widget::DoModal();
	return m_ReturnCode;
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
