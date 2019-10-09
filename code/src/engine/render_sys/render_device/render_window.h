#pragma once

#ifndef AE_RENDER_SYS_RENDER_WINDOW_H
#define AE_RENDER_SYS_RENDER_WINDOW_H

#ifdef AE_RENDER_DX11
	#include "dx11/render_window_dx11.h"
#else
	#error No render window available !
#endif // #ifdef AE_RENDER_DX11

namespace ae { namespace input {
	class I_WindowCallback;
} } // namespace ae { namespace input {

namespace ae { namespace render {

	class C_RenderSys;

	class C_RenderWindow
	{
		friend class ae::render::C_RenderSys;
		dx11::C_RenderWindow* m_Impl;
// *****************************************************************************************************
		C_RenderWindow();
		~C_RenderWindow();
// *****************************************************************************************************
	public:
	#ifdef AE_RENDER_DX11
		dx11::C_RenderWindow* GetImpl() { return m_Impl; }
	#endif // #ifdef AE_RENDER_DX11
// *****************************************************************************************************
		void RegisterWindowCallback(ae::input::I_WindowCallback*);
		void UnregisterWindowCallback(ae::input::I_WindowCallback*);
// *****************************************************************************************************
		void SetWindowDimensions(ae::u32 windowWidth, ae::u32 windowHeight);
		void GetWindowDimensions(ae::u32& windowWidth, ae::u32& windowHeight) const;
// *****************************************************************************************************
		void HideAndLockCursor(bool bHideAndLock);
		bool IsCursorHiddenAndLocked() const;
// *****************************************************************************************************
	private:
		const C_RenderWindow& operator = (const C_RenderWindow&);
		C_RenderWindow(const C_RenderWindow&);
	};

} } // namespace ae { namespace render {

#endif // #ifdef AE_RENDER_SYS_RENDER_WINDOW_H
