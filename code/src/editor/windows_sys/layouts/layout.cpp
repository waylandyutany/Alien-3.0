#include "stdafx.h"

#if defined(AE_EDITOR)

#include "layout.h"
#include "../widgets/widget.h"

namespace ae { namespace windows_sys {

C_Layout::C_Layout() : m_Marging(0,0), m_Spacing(8,8)
{
}

/*virtual*/C_Layout::~C_Layout()
{
}

/*virtual*/ void C_Layout::GetLayoutRect(S_Rect& layoutRect)
{
	layoutRect = m_LayoutRect;
	layoutRect.Inflate(m_Marging.width, m_Marging.height);
}

/*virtual*/ void C_Layout::SetLayoutRect(const S_Rect& rect)
{
	m_LayoutRect = rect;
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
