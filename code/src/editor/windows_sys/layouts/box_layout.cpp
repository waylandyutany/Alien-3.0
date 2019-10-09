#include "stdafx.h"

#if defined(AE_EDITOR)

#include "box_layout.h"
#include "../widgets/widget.h"

namespace ae { namespace windows_sys {

C_BoxLayout::C_BoxLayout(int size) : m_Size(size)
{
}

/*virtual*/ void C_BoxLayout::SetLayoutRect(const S_Rect& rect)
{
	C_Layout::SetLayoutRect(rect);
}

void C_BoxLayout::AddLayoutItem(C_LayoutItem* pLayoutItem, int size, int stretchx, int stretchy)
{
	if(std::find_if(m_LayoutItems.begin(), m_LayoutItems.end(), [pLayoutItem](const S_LayoutItem& li) { return li.pLayoutItem == pLayoutItem; }) == m_LayoutItems.end())
		m_LayoutItems.push_back(S_LayoutItem(pLayoutItem, size, stretchx, stretchy));
}

void C_BoxLayout::RemoveLayoutItem(C_LayoutItem* pLayoutItem)
{
	m_LayoutItems.erase( std::remove_if(m_LayoutItems.begin(), m_LayoutItems.end(), [pLayoutItem](const S_LayoutItem& li) { return li.pLayoutItem == pLayoutItem; }));
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
