#pragma once
#if !defined(AE_WINDOW_H) && defined(AE_EDITOR)
#define AE_WINDOW_H

#if (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#include "win32/window_win32.h"
#elif
	#error No window platform implemented !
#endif

#include "menu.h"
#include "structs.h"

#include "window_constants.h"

namespace ae { namespace windows_sys {

	class C_Window : public T_WindowImpl
	{
	protected:
		C_Window* m_pParent;
		NOCOPY_CLASS(C_Window);

		virtual void OnDestroy() {};
		virtual void OnClose() {};
		virtual void OnCommand(T_ControlID) {};

		C_Window(T_WindowStyle widgetStyle = windowStyle_Visible | windowStyle_TiledWindow | windowStyle_ClipChildrens | windowStyle_ClipSiblings);
		C_Window(C_Window*, T_WindowStyle widgetStyle = windowStyle_Visible | windowStyle_Child | windowStyle_ClipChildrens | windowStyle_ClipSiblings | windowStyle_DlgFrame);
		C_Window(C_Window*, T_WindowType, T_WindowStyle, T_ControlID);
		virtual ~C_Window();
	public:
		void SetMenu(C_Menu& menu);
		void RemoveMenu();
// *****************************************************************************************************
		void ShowWindow(bool bShow);
		bool IsVisible() const;
		void EnableWindow(bool bEnable);
		bool IsWindowEnabled() const;
// *****************************************************************************************************
		void SetText(const char*);
// *****************************************************************************************************
		void GetRect(S_Rect& rect, bool clientRect = false) const;
		void SetRect(const S_Rect& rect, bool clientRect = false);
		void SetSize(ae::u32 width, ae::u32 height);
		void SetPos(ae::u32 x, ae::u32 y);
	};

} } // namespace ae { namespace windows_sys {

#include "window.inl"

#endif // #if !defined(AE_WINDOW_H) && defined(AE_EDITOR)
