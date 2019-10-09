namespace ae { namespace windows_sys {

	// *****************************************************************************************************
	inline void C_NumberSpinCtrl::SetWheelBoost(ae::u32 boost) {
		m_Impl.SetWheelBoost(boost);
	}
	inline ae::u32 C_NumberSpinCtrl::GetWheelBoost() const {
		return m_Impl.GetWheelBoost();
	}

	inline void C_NumberSpinCtrl::LoopValue(bool bLoopValue) {
		m_Impl.Params().LoopValue(bLoopValue);
	}
	inline bool C_NumberSpinCtrl::IsValueLooped() const {
		return m_Impl.Params().IsValueLooped();
	}
	inline void C_NumberSpinCtrl::SetFloatPrecision(int fltPrecision) {
		m_Impl.Params().SetFloatPrecision(fltPrecision);
	}
	inline int C_NumberSpinCtrl::GetFloatPrecision() const {
		return m_Impl.Params().GetFloatPrecision();
	}
	// *****************************************************************************************************
	inline void C_NumberSpinCtrl::SetMinMaxFloat(float min, float max) {
		m_Impl.Params().SetMinMaxFloat(min, max);
	}
	inline float C_NumberSpinCtrl::GetMaxFloat() const {
		return m_Impl.Params().GetMaxFloat();
	}
	inline float C_NumberSpinCtrl::GetMinFloat() const {
		return m_Impl.Params().GetMinFloat();
	}
	inline void C_NumberSpinCtrl::SetStepFloat(float step) {
		m_Impl.Params().SetStepFloat(step);
	}
	inline float C_NumberSpinCtrl::GetStepFloat() const {
		return m_Impl.Params().GetStepFloat();
	}
	inline void C_NumberSpinCtrl::SetValueFloat(float value) {
		m_Impl.Params().SetValueFloat(value);
	}
	inline float C_NumberSpinCtrl::GetValueFloat() const {
		return m_Impl.Params().GetValueFloat();
	}
	// *****************************************************************************************************
	inline void C_NumberSpinCtrl::SetMinMaxInteger(T_Integer min, T_Integer max) {
		m_Impl.Params().SetMinMaxInteger(min, max);
	}
	inline T_Integer C_NumberSpinCtrl::GetMaxInteger() const {
		return m_Impl.Params().GetMaxInteger();
	}
	inline T_Integer C_NumberSpinCtrl::GetMinInteger() const {
		return m_Impl.Params().GetMinInteger();
	}
	inline void C_NumberSpinCtrl::SetStepInteger(T_Integer step) {
		m_Impl.Params().SetStepInteger(step);
	}
	inline T_Integer C_NumberSpinCtrl::GetStepInteger() const {
		return m_Impl.Params().GetStepInteger();
	}
	inline void C_NumberSpinCtrl::SetValueInteger(T_Integer value) {
		m_Impl.Params().SetValueInteger(value);
	}
	inline T_Integer C_NumberSpinCtrl::GetValueInteger() const {
		return m_Impl.Params().GetValueInteger();
	}
	// *****************************************************************************************************

} } // namespace ae { namespace windows_sys {
