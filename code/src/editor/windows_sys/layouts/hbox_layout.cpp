#include "stdafx.h"

#if defined(AE_EDITOR)

#include "hbox_layout.h"
#include "../widgets/widget.h"

namespace ae { namespace windows_sys {

C_HBoxLayout::C_HBoxLayout(int height) : C_BoxLayout(height)
{
}

/*virtual*/ void C_HBoxLayout::SetLayoutRect(const S_Rect& rect)
{
	S_Rect layoutRect;
	C_BoxLayout::SetLayoutRect(rect);
	GetLayoutRect(layoutRect);

// ******************************************************************************************************
// Calculates total items horizontal size and total horizontal stretching
	size_t totalItemsWidth = 0;
	size_t totalItemsStretchedWidth = 0;
	for(auto i(m_LayoutItems.begin());i != m_LayoutItems.end(); ++i)
	{
		totalItemsWidth += (*i).size;
		totalItemsStretchedWidth += (*i).stretchx;
	}
	if(m_LayoutItems.size()) // add spacing factor as well
		totalItemsWidth += (m_LayoutItems.size() - 1) * m_Spacing.width;
// ******************************************************************************************************
// Stretch X factor
	size_t maxStretchX = layoutRect.Width() - totalItemsWidth;
	if(maxStretchX < 0) maxStretchX = 0;
// ******************************************************************************************************
// Stretch Y factor
	size_t maxStretchY = layoutRect.Height() - m_Size;
	if(maxStretchY < 0) maxStretchY = 0;
// ******************************************************************************************************
	
	int left = layoutRect.left;
	for(auto i(m_LayoutItems.begin());i != m_LayoutItems.end(); ++i)
	{

		// stretch x is based on ration between item related stretch x and total stretch we can get
		int stretchX = totalItemsStretchedWidth == 0 ? 0 : (int)(((*i).stretchx / (float)totalItemsStretchedWidth) * maxStretchX);
		// clamping stretch to not exceed item related stretch y
		stretchX = stretchX > (*i).stretchx ? (*i).stretchx : stretchX;

		// since we have only one item in vertival we simple clamp stretchx to it's item related stretch x
		const int stretchY = (int)((*i).stretchy > maxStretchY ? maxStretchY : (*i).stretchy);

		(*i).pLayoutItem->SetLayoutRect(S_Rect(left, layoutRect.top, left+(*i).size+stretchX, layoutRect.top + m_Size + stretchY));
		left += (*i).size + m_Spacing.width + stretchX;
	}
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
