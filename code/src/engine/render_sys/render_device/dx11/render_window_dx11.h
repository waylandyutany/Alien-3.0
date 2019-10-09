#pragma once

#ifndef AE_RENDER_SYS_RENDER_WINDOW_DX11_H
#define AE_RENDER_SYS_RENDER_WINDOW_DX11_H

namespace ae { namespace render {
	class C_RenderWindow;
} } // namespace ae { namespace render {

namespace ae { namespace input {
	class I_WindowCallback;
} } // namespace ae { namespace input {

namespace ae { namespace render { namespace dx11 {

	class C_RenderWindow
	{
		C_RenderWindow();
		~C_RenderWindow();

		friend class ae::render::C_RenderWindow;

		HWND m_hWnd;
		ae::input::I_WindowCallback* m_pWindowCallback;
	public:
		HWND GetHWnd() const { return m_hWnd; }
// *****************************************************************************************************
		void RegisterWindowCallback(ae::input::I_WindowCallback*);
		void UnregisterWindowCallback(ae::input::I_WindowCallback*);
// *****************************************************************************************************
		void SetWindowDimensions(ae::u32 windowWidth, ae::u32 windowHeight);
		void GetWindowDimensions(ae::u32 windowWidth, ae::u32 windowHeight) const;
// *****************************************************************************************************
		void HideAndLockCursor(bool bHideAndLock);
		bool IsCursorHiddenAndLocked() const;
// *****************************************************************************************************
	};

} } } // namespace ae { namespace render { namespace dx11 {

#endif // #ifdef AE_RENDER_SYS_RENDER_WINDOW_DX11_H
