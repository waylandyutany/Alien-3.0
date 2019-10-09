#pragma once

#ifndef AE_INPUT_SYS_WIN32_H
#define AE_INPUT_SYS_WIN32_H

#include "../key_codes.h"
#include "../../math_sys/types.h"

namespace ae { namespace input {
	class C_InputSys;
} } // namespace ae { namespace input {

namespace ae { namespace input { namespace dxinput8 {

	class C_InputSys
	{
		HWND m_hMainWindow;
		IDirectInput8* m_DirectInput;
		IDirectInputDevice8* m_KeyboardDevice;
		IDirectInputDevice8* m_MouseDevice;
		bool m_bIsFrozen;

		typedef unsigned char T_KeyboardState[256];
		typedef DIMOUSESTATE2 T_MouseState;

		T_KeyboardState m_PreviousKeyboardState,m_KeyboardState;
		T_MouseState m_PreviousMouseState, m_MouseState;

		~C_InputSys();
		NOCOPY_CLASS(C_InputSys);
		void ProcessMainWindowMessage(MSG&);
		C_InputSys(HWND);

		friend class ae::input::C_InputSys;

		void ProcessMouse();
		void ProcessKeyboard();
		const char* GetProviderName() const;
	public:
		bool HandleMsgPump();
		void Update();

		bool KeyState(E_Key_State, E_Key_Code);
		bool MouseState(E_Key_State, E_Mouse_Button);

		ae::math::float3 MouseMove() const;
		ae::math::float2 MouseCursorPos() const;

		void Freeze(bool bFreeze);
		bool IsFrozen() const;

		void Quit();
	};

} } } // namespace ae { namespace input { namespace dxinput8 {

#include "input_dxinput8.inl"

#endif // #ifdef AE_INPUT_SYS_WIN32_H
