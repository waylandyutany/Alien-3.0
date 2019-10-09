#pragma once
#if !defined(AE_NUMBER_SPINS_H) && defined(AE_EDITOR)
#define AE_NUMBER_SPINS_H

#include "widget.h"
#include "../base/controls/number_spins_ctrl.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
	class C_NumberSpinCallback : public C_BaseWidgetCallback
	{
	public:
		virtual void OnFloatChange(float, T_ControlID) {};
		virtual void OnIntegerChange(T_Integer, T_ControlID) {};
	};
// *****************************************************************************************************
// *****************************************************************************************************
	class C_NumberSpin : public C_Widget
	{
		C_NumberSpin();
		NOCOPY_CLASS(C_NumberSpin);
	protected:
		C_NumberSpinCtrl m_Control;
		C_NumberSpin(E_NumberSpinType, C_NumberSpinCallback*, C_Widget* pParent, T_ControlID);
	public:
		void LoopValue(bool);
		bool IsValueLooped() const;
		void SetWheelBoost(ae::u32);
		ae::u32 GetWheelBoost() const;
	};
	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_FloatSpin : public C_NumberSpin
	{
	public:
		C_FloatSpin(C_NumberSpinCallback*, C_Widget* pParent, T_ControlID);

		void SetMinMax(float min, float max);
		float GetMax() const;
		float GetMin() const;
		void SetStep(float);
		float GetStep() const;
		void SetValue(float);
		float GetValue() const;

		void SetFloatPrecision(int);
		int GetFloatPrecision() const;
	};
	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_IntegerSpin : public C_NumberSpin
	{
	public:
		C_IntegerSpin(C_NumberSpinCallback*, C_Widget* pParent, T_ControlID);

		void SetMinMax(T_Integer min, T_Integer max);
		T_Integer GetMax() const;
		T_Integer GetMin() const;
		void SetStep(T_Integer);
		T_Integer GetStep() const;
		void SetValue(T_Integer);
		T_Integer GetValue() const;
	};
	// *****************************************************************************************************
	// *****************************************************************************************************
} } // namespace ae { namespace windows_sys {

#include "number_spins.inl"

#endif // #if !defined(AE_NUMBER_SPINS_H) && defined(AE_EDITOR)
