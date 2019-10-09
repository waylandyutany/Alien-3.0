#pragma once
#if !defined(AE_WIDGET_H) && defined(AE_EDITOR)
#define AE_WIDGET_H

#include "../layouts/layout_item.h"
#include "../base/window.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
	class C_BaseWidgetCallback
	{
		public:
			C_BaseWidgetCallback() {};
			virtual ~C_BaseWidgetCallback() {};
	};
// *****************************************************************************************************
	class C_Layout;
// *****************************************************************************************************
// *****************************************************************************************************
	class C_Widget : public C_Window, public C_LayoutItem
	{
		NOCOPY_CLASS(C_Widget);
		C_Layout* m_pLayout;

	protected:
		C_Widget(T_WindowStyle widgetStyle = windowStyle_Visible | windowStyle_TiledWindow | windowStyle_ClipChildrens | windowStyle_ClipSiblings);
		C_Widget(C_Widget* pParent, T_WindowStyle widgetStyle = windowStyle_Visible | windowStyle_Child | windowStyle_ClipChildrens | windowStyle_ClipSiblings | windowStyle_DlgFrame);
		C_Widget(C_Widget* pParent, T_WindowType, T_WindowStyle, T_ControlID);

		virtual void OnSize(ae::u32 width, ae::u32 height);

		virtual void GetLayoutRect(S_Rect& layoutRect);
		virtual void SetLayoutRect(const S_Rect& layoutRect);
	public:
		virtual ~C_Widget();

		void SetLayout(C_Layout*);
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_WIDGET_H) && defined(AE_EDITOR)
