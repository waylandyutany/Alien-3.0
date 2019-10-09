#include "stdafx.h"

#if defined(AE_EDITOR)

#include "vbox_layout.h"
#include "../widgets/widget.h"

namespace ae { namespace windows_sys {

C_VBoxLayout::C_VBoxLayout(int width) : C_BoxLayout(width)
{
}

/*virtual*/ void C_VBoxLayout::SetLayoutRect(const S_Rect& rect)
{
	S_Rect layoutRect;
	C_BoxLayout::SetLayoutRect(rect);
	GetLayoutRect(layoutRect);
// ******************************************************************************************************
// Calculates total items vertical size and total vertical stretching
	size_t totalItemsHeight = 0;
	size_t totalItemsStretchedHeight = 0;
	for(auto i(m_LayoutItems.begin());i != m_LayoutItems.end(); ++i)
	{
		totalItemsHeight += (*i).size;
		totalItemsStretchedHeight += (*i).stretchy;
	}
	if(m_LayoutItems.size()) // add spacing factor as well
		totalItemsHeight += (m_LayoutItems.size() - 1) * m_Spacing.height;
// ******************************************************************************************************
// Stretch Y factor
	size_t maxStretchY = layoutRect.Height() - totalItemsHeight;
	if(maxStretchY < 0) maxStretchY = 0;
// ******************************************************************************************************
// Stretch X factor
	size_t maxStretchX = layoutRect.Width() - m_Size;
	if(maxStretchX < 0) maxStretchX = 0;
// ******************************************************************************************************
	int top = layoutRect.top;
	for(auto i(m_LayoutItems.begin());i != m_LayoutItems.end(); ++i)
	{
		// stretch y is based on ration between item related stretch y and total stretch we can get
		int stretchY = (int)totalItemsStretchedHeight == 0 ? 0 : (int)(((*i).stretchy / (float)totalItemsStretchedHeight) * maxStretchY);
		// clamping stretch to not exceed item related stretch y
		stretchY = (int)stretchY > (*i).stretchy ? (*i).stretchy : stretchY;
		// since we have only one item in vertival we simple clamp stretchx to it's item related stretch x
		const int stretchX = (int)((*i).stretchx > maxStretchX ? maxStretchX : (*i).stretchx);

		(*i).pLayoutItem->SetLayoutRect(S_Rect(layoutRect.left, top, layoutRect.left + m_Size + stretchX, top + (*i).size + stretchY));
		top += (*i).size + m_Spacing.height + stretchY;
	}
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
