#pragma once
#if !defined(AE_WINDOW_BOX_LAYOUT_H) && defined(AE_EDITOR)
#define AE_WINDOW_BOX_LAYOUT_H

#include "layout.h"
#include "layout_item.h"

namespace ae { namespace windows_sys {

	class C_BoxLayout : public C_Layout
	{
		C_BoxLayout();
	protected:
		int m_Size;
// *****************************************************************************************************
		struct S_LayoutItem
		{
			int size, stretchx, stretchy;
			C_LayoutItem* pLayoutItem;
			S_LayoutItem(C_LayoutItem* _pLayoutItem, int _size, int _stretchx, int _stretchy) : pLayoutItem(_pLayoutItem), size(_size), stretchx(_stretchx), stretchy(_stretchy) {}
		private:
			S_LayoutItem();
		};
		typedef std::deque<S_LayoutItem> T_LayoutInfos;
		T_LayoutInfos m_LayoutItems;
// *****************************************************************************************************
		C_BoxLayout(int size);
	public:
// *****************************************************************************************************
		void SetSize(int size) { m_Size = size; }
		int GetSize() const { return m_Size; }
// *****************************************************************************************************
		void AddLayoutItem(C_LayoutItem*, int size, int stretchx = 0, int stretchy = 0);
		void RemoveLayoutItem(C_LayoutItem*);
// *****************************************************************************************************
// *****************************************************************************************************
		virtual void SetLayoutRect(const S_Rect& layoutRect);
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_WINDOW_BOX_LAYOUT_H) && defined(AE_EDITOR)
