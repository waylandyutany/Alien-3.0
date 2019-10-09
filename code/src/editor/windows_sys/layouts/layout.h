#pragma once
#if !defined(AE_WINDOW_LAYOUT_H) && defined(AE_EDITOR)
#define AE_WINDOW_LAYOUT_H

#include "../base/structs.h"
#include "layout_item.h"

namespace ae { namespace windows_sys {

	class C_Widget;
// *****************************************************************************************************
// *****************************************************************************************************
	class C_Layout : public C_LayoutItem
	{
		S_Rect m_LayoutRect;
	protected:
		S_Size m_Marging;
		S_Size m_Spacing;
		C_Layout();
	public:
		virtual ~C_Layout();
// *****************************************************************************************************
		virtual void GetLayoutRect(S_Rect& layoutRect);
		virtual void SetLayoutRect(const S_Rect& layoutRect);
// *****************************************************************************************************
		void SetSpacing(int spx, int spy) { m_Spacing.width = spx; m_Spacing.height = spy;}
		void SetMarging(int mcx, int mcy) { m_Marging.width = mcx; m_Marging.height = mcy; }
// *****************************************************************************************************
		const S_Size& GetSpacing() const { return m_Spacing; }
		const S_Size& GetMarging() const { return m_Marging; }
// *****************************************************************************************************
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_WINDOW_LAYOUT_H) && defined(AE_EDITOR)
