#pragma once
#if !defined(AE_WINDOW_HBOX_LAYOUT_H) && defined(AE_EDITOR)
#define AE_WINDOW_HBOX_LAYOUT_H

#include "box_layout.h"

namespace ae { namespace windows_sys {

	class C_HBoxLayout : public C_BoxLayout
	{
	public:
		C_HBoxLayout(int height);
		virtual void SetLayoutRect(const S_Rect& layoutRect);
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_WINDOW_HBOX_LAYOUT_H) && defined(AE_EDITOR)
