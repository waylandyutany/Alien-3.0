#pragma once
#if !defined(AE_NUMBER_SPINS_CTRL_H) && defined(AE_EDITOR)
#define AE_NUMBER_SPINS_CTRL_H

#if (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#include "../win32/controls/number_spins_ctrl_win32.h"
#elif
	#error No window platform implemented !
#endif

#include "../window.h"
#include "number_spins_type.h"

namespace ae { namespace windows_sys {

	class C_NumberSpinCallback;
	// *****************************************************************************************************
	// *****************************************************************************************************

	class C_NumberSpinCtrl
	{
		C_NumberSpinCtrl();
		NOCOPY_CLASS(C_NumberSpinCtrl);
		T_NumberSpinCtrl_Impl m_Impl;
	public:
		C_NumberSpinCtrl(E_NumberSpinType,C_NumberSpinCallback*, C_Window& control);
	// *****************************************************************************************************
		void LoopValue(bool);
		bool IsValueLooped() const;
		void SetFloatPrecision(int);
		int GetFloatPrecision() const;
		void SetWheelBoost(ae::u32);
		ae::u32 GetWheelBoost() const;
	// *****************************************************************************************************
		void SetMinMaxFloat(float min, float max);
		float GetMaxFloat() const;
		float GetMinFloat() const;
		void SetStepFloat(float);
		float GetStepFloat() const;
		void SetValueFloat(float);
		float GetValueFloat() const;
	// *****************************************************************************************************
		void SetMinMaxInteger(T_Integer min, T_Integer max);
		T_Integer GetMaxInteger() const;
		T_Integer GetMinInteger() const;
		void SetStepInteger(T_Integer);
		T_Integer GetStepInteger() const;
		void SetValueInteger(T_Integer);
		T_Integer GetValueInteger() const;
	// *****************************************************************************************************
	};

} } // namespace ae { namespace windows_sys {

#include "number_spins_ctrl.inl"

#endif // #if !defined(AE_NUMBER_SPINS_CTRL_H) && defined(AE_EDITOR)
