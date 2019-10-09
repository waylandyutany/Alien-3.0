#pragma once

#ifndef AE_INPUT_SYS_H
#define AE_INPUT_SYS_H

#ifdef AE_INPUT_DXINPUT8
#include "dxinput8/input_dxinput8.h"
#else
	#error No input available !
#endif // #ifdef AE_AUDIO_XAUDIO2

#include "key_codes.h"

#include "input_sys_init.h"

#include "../math_sys/types.h"

namespace ae { namespace input {

	class C_InputSys
	{
		static C_InputSys* pInstance;

#ifdef AE_INPUT_DXINPUT8
		dxinput8::C_InputSys m_Impl;
#endif // #ifdef AE_INPUT_DXINPUT8

		C_InputSys(S_Init&);
		C_InputSys();
		NOCOPY_CLASS(C_InputSys);

		const char* GetProviderName() const;
	public:
		bool HandleMsgPump();

		bool KeyState(E_Key_State, E_Key_Code);
		bool MouseState(E_Key_State, E_Mouse_Button);

		ae::math::float3 MouseMove() const;

		// returns cursor position relative to main window
		// use with IsFrozen to detect whether main window has it's focus
		ae::math::float2 MouseCursorPos() const;

		void Update();

		void Freeze(bool bFreeze);
		bool IsFrozen() const;
		void Quit();
	public:
		static bool Create(S_Init&);
		static void Release();
		static C_InputSys& Inst();
	};

} } // namespace ae { namespace input {

#include "input_sys.inl"

#endif // #ifdef AE_INPUT_SYS_H
