#pragma once
#if !defined(AE_LAYOUT_ITEM_H) && defined(AE_EDITOR)
#define AE_LAYOUT_ITEM_H

#include "../base/structs.h"

namespace ae { namespace windows_sys {

	class C_LayoutItem
	{
	protected:
		C_LayoutItem();
	public:
		virtual ~C_LayoutItem();

		virtual void GetLayoutRect(S_Rect& layoutRect) = 0;
		virtual void SetLayoutRect(const S_Rect& layoutRect) = 0;
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_LAYOUT_ITEM_H) && defined(AE_EDITOR)
