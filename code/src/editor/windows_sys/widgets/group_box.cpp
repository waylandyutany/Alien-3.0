#include "stdafx.h"

#if defined(AE_EDITOR)

#include "group_box.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
C_GroupBox::C_GroupBox(C_Widget* pParent, const char* sTitle) :
	C_Button(pParent, sTitle, windowStyle_Visible | windowStyle_TabStop | windowStyle_Child | windowStyle_GroupBox, 0)
{
}

/*virtual*/ void C_GroupBox::GetLayoutRect(S_Rect& layoutRect)
{
	C_Widget::GetLayoutRect(layoutRect);

	layoutRect.left += 8;
	layoutRect.right -= 8;
	layoutRect.top += 16;
	layoutRect.bottom -= 16;
}


} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
