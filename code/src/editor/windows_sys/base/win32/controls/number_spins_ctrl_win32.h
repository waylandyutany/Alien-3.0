#pragma once
#if !defined(AE_NUMBER_SPINS_CTRL_WIN32_H) && defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#define AE_NUMBER_SPINS_CTRL_WIN32_H

#include "base_ctrl_win32.h"
#include "../../controls/number_spins_type.h"
#include "../../win32/window_win32.h"

namespace ae { namespace windows_sys {
	class C_NumberSpinCallback;
} } // namespace ae { namespace windows_sys {

namespace ae { namespace windows_sys {  namespace win32 {
	// *****************************************************************************************************
	// *****************************************************************************************************
	typedef ae::s32 T_Integer;

	class C_NumberSpinCtrl_Win32 : public C_BaseCtrl_Win32
	{
		C_NumberSpinCtrl_Win32();
		NOCOPY_CLASS(C_NumberSpinCtrl_Win32);

// *****************************************************************************************************
		enum E_State
		{
			E_None,
			E_MouseOverPlus,
			E_MouseOverMinus,
			E_MouseOverBoth,
			E_MousePressedPlus,
			E_MousePressedMinus,
			E_MousePressedBoth
		};
		E_State state;

		struct S_Params
		{
			S_Params(C_NumberSpinCtrl_Win32&, ae::windows_sys::E_NumberSpinType type);

			void LoopValue(bool);
			bool IsValueLooped() const;

			void SetFloatPrecision(int);
			int GetFloatPrecision() const;

			void SetMinMaxFloat(float min, float max);
			float GetMaxFloat() const;
			float GetMinFloat() const;
			void SetStepFloat(float);
			float GetStepFloat() const;
			void SetValueFloat(float, bool notify = false, bool updateText = true);
			float GetValueFloat() const;

			void SetMinMaxInteger(T_Integer min, T_Integer max);
			T_Integer GetMaxInteger() const;
			T_Integer GetMinInteger() const;
			void SetStepInteger(T_Integer);
			T_Integer GetStepInteger() const;
			void SetValueInteger(T_Integer, bool notify = false, bool updateText = true);
			T_Integer GetValueInteger() const;

			void ChangeValue(long delta);
			void SetValue(const char*);
		private:
			S_Params();
			union {
				float minFloat;
				T_Integer minInteger;
			};
			union {
				float maxFloat;
				T_Integer maxInteger;
			};
			union {
				float stepFloat;
				T_Integer stepInteger;
			};
			union {
				float valueFloat;
				T_Integer valueInteger;
			};
			ae::windows_sys::E_NumberSpinType type;
			bool bLoopValue;
			int floatPrecision;
			C_NumberSpinCtrl_Win32& control;
		};

		S_Params params;
		friend struct S_Params;
// *****************************************************************************************************
		C_NumberSpinCallback* pCallback;
		HWND editBoxHWND;
		RECT clientRect;
		UINT timeToSinglePress, timeToAutoPress;
		POINT mouseDragPos;
		ae::u32 wheelDeltaBoost;

		void NotityPlusMinus();
		void UpdateMouseState(POINT& mousePos, bool bPressed);
		void UpdateMouseState(bool bPressed);
		void UpdateMouseCursor();
		bool IsMousePressed() const;
		void DrawSmartSpin(HDC hdc);
		void CalculateRectangles(RECT* editRect, RECT* plusRect, RECT* minusRect, RECT* dragRect);

		void DrawMinusSpinArrow(HDC hdc, RECT rect, HPEN pen);
		void DrawPlusSpinArrow(HDC hdc, RECT rect, HPEN pen);
		void DrawSpinWindow(HDC hdc, RECT rect, HPEN ltPen, HPEN rbPen, HPEN bdPen);
		void DrawSpin(HDC hdc, RECT rect, bool isPressed, bool isSelected, bool isMinus);

		virtual S_WPReturn WndProc(UINT umsg, WPARAM wparam, LPARAM lparam, S_WPReturn result);
	public:
		C_NumberSpinCtrl_Win32(ae::windows_sys::E_NumberSpinType, C_NumberSpinCallback*, C_Window_Win32& control);
		~C_NumberSpinCtrl_Win32();
		S_Params& Params() { return params; }
		const S_Params& Params() const { return params; }

		void SetWheelBoost(ae::u32);
		ae::u32 GetWheelBoost() const;
	};
	// *****************************************************************************************************
	// *****************************************************************************************************
} } } // namespace ae { namespace windows_sys {  namespace win32 {

#include "number_spins_ctrl_win32.inl"

namespace ae { namespace windows_sys {
	typedef ae::windows_sys::win32::C_NumberSpinCtrl_Win32 T_NumberSpinCtrl_Impl;
} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_NUMBER_SPINS_CTRL_WIN32_H) && defined(AE_EDITOR)
