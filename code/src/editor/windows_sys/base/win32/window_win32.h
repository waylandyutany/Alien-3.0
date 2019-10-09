#pragma once
#if !defined(AE_WINDOW_WIN32_H) && defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#define AE_WINDOW_WIN32_H

#include "menu_win32.h"

#include "../structs.h"
#include "../window_constants.h"

namespace ae { namespace windows_sys {  namespace win32 {

	class C_BaseCtrl_Win32;

	class C_Window_Win32
	{
		HWND m_hWnd;
		WNDPROC m_WndProc;
		C_Window_Win32* m_pParent;
		C_BaseCtrl_Win32* m_pBaseControl;
		const T_ControlID m_ControlID;
		bool m_bDoModal;
// *****************************************************************************************************
		typedef std::deque<C_BaseCtrl_Win32*> T_Controls;
		T_Controls m_Controls;
		friend class C_BaseCtrl_Win32;
		void AddControl(C_BaseCtrl_Win32*);
		void RemoveControl(C_BaseCtrl_Win32*);
// *****************************************************************************************************
		void EnableDragAndDrop();
		void DisableDragAndDrop();
		void DefaultWindowInitialization();
// *****************************************************************************************************
		NOCOPY_CLASS(C_Window_Win32);
	public:
		operator HWND() const { return m_hWnd; }
		HWND GetHWnd() const { return  m_hWnd; }
		T_ControlID GetControlID() const { return m_ControlID; }
		C_Window_Win32* GetParent() { return m_pParent; }
		void SetBaseControl(C_BaseCtrl_Win32* pBaseControl) { m_pBaseControl = pBaseControl; }

		C_Window_Win32(T_WindowStyle widgetStyle = windowStyle_Visible | windowStyle_TiledWindow | windowStyle_ClipChildrens | windowStyle_ClipSiblings);
		C_Window_Win32(C_Window_Win32* pParent, T_WindowStyle widgetStyle = windowStyle_Visible | windowStyle_Child | windowStyle_ClipChildrens | windowStyle_ClipSiblings | windowStyle_DlgFrame);
		C_Window_Win32(C_Window_Win32* pParent, T_WindowType, T_WindowStyle, T_ControlID);
		virtual ~C_Window_Win32();
	protected:
		void DoModal();
		void EndModal();
		void CloseWindow();
	public:
		LRESULT CallDefaultWndProc(UINT umsg, WPARAM wparam, LPARAM lparam);
		LRESULT WndProc(UINT umsg, WPARAM wparam, LPARAM lparam);
// *****************************************************************************************************
		virtual void OnDestroy() {};
		virtual void OnClose() {};
		virtual void OnCommand(T_ControlID) {};
		virtual void OnSize(::ae::u32 width, ::ae::u32 height) {};
		virtual void OnTimer(ae::u32 timerID) {};
// *****************************************************************************************************
		void SetMenu(C_Menu_Win32& menu);
		void RemoveMenu();
// *****************************************************************************************************
		void ShowWindow(bool bShow);
		bool IsVisible() const;
// *****************************************************************************************************
		void EnableWindow(bool bEnable);
		bool IsWindowEnabled() const;
// *****************************************************************************************************
		void SetTimer(ae::u32 timerID, ae::u32 timerMiliseconds);
		void DelTimer(ae::u32 timerID);
// *****************************************************************************************************
		void SetText(const char*);
		std::string GetText() const;
// *****************************************************************************************************
		void GetRect(S_Rect& rect, bool clientRect = false) const;
		void SetRect(const S_Rect& rect, bool clientRect = false);
		void SetSize(::ae::u32 width, ::ae::u32 height);
		void SetPos(::ae::u32 x, ::ae::u32 y);
		void ClientToScreen(S_Point&);
		void ScreenToClient(S_Point&);
	};

} } } // namespace ae { namespace windows_sys {  namespace win32 {

namespace ae { namespace windows_sys {
	typedef ae::windows_sys::win32::C_Window_Win32 T_WindowImpl;
} } // namespace ae { namespace windows_sys {

#include "window_win32.inl"

#endif // #if !defined(AE_WINDOW_WIN32_H) && defined(AE_EDITOR)
