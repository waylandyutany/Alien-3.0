namespace ae { namespace windows_sys {  namespace win32 {
// *****************************************************************************************************
// *****************************************************************************************************

	inline void C_NumberSpinCtrl_Win32::SetWheelBoost(ae::u32 boost) {
		wheelDeltaBoost = boost;
		if(wheelDeltaBoost < 1)
			wheelDeltaBoost = 1;
	}
	inline ae::u32 C_NumberSpinCtrl_Win32::GetWheelBoost() const {
		return wheelDeltaBoost;
	}

// *****************************************************************************************************
// *****************************************************************************************************
	inline void C_NumberSpinCtrl_Win32::S_Params::SetMinMaxFloat(float min, float max) {
		minFloat = min; maxFloat = max;
		SetValueFloat(GetValueFloat(), true);
	}
	inline float C_NumberSpinCtrl_Win32::S_Params::GetMaxFloat() const {
		return maxFloat;
	}
	inline float C_NumberSpinCtrl_Win32::S_Params::GetMinFloat() const {
		return minFloat;
	}
	inline void C_NumberSpinCtrl_Win32::S_Params::SetStepFloat(float step) {
		stepFloat = step;
	}
	inline float C_NumberSpinCtrl_Win32::S_Params::GetStepFloat() const {
		return stepFloat;
	}
	inline float C_NumberSpinCtrl_Win32::S_Params::GetValueFloat() const {
		return valueFloat;
	}

// *****************************************************************************************************
// *****************************************************************************************************
	inline void C_NumberSpinCtrl_Win32::S_Params::SetMinMaxInteger(T_Integer min, T_Integer max) {
		minInteger = min; maxInteger = max;
		SetValueInteger(GetValueInteger(), true);
	}
	inline T_Integer C_NumberSpinCtrl_Win32::S_Params::GetMaxInteger() const {
		return maxInteger;
	}
	inline T_Integer C_NumberSpinCtrl_Win32::S_Params::GetMinInteger() const {
		return minInteger;
	}
	inline void C_NumberSpinCtrl_Win32::S_Params::SetStepInteger(T_Integer step) {
		stepInteger = step;
	}
	inline T_Integer C_NumberSpinCtrl_Win32::S_Params::GetStepInteger() const {
		return stepInteger;
	}
	inline T_Integer C_NumberSpinCtrl_Win32::S_Params::GetValueInteger() const {
		return valueInteger;
	}

	// *****************************************************************************************************
	// *****************************************************************************************************
	inline void C_NumberSpinCtrl_Win32::S_Params::LoopValue(bool bLoop) {
		bLoopValue = bLoop;
	}
	inline bool C_NumberSpinCtrl_Win32::S_Params::IsValueLooped() const {
		return bLoopValue;
	}

	inline void C_NumberSpinCtrl_Win32::S_Params::SetFloatPrecision(int fltPrecision) {
		floatPrecision = fltPrecision;
	}
	inline int C_NumberSpinCtrl_Win32::S_Params::GetFloatPrecision() const {
		return floatPrecision;
	}
	// *****************************************************************************************************
	// *****************************************************************************************************

} } } // namespace ae { namespace windows_sys {  namespace win32 {
