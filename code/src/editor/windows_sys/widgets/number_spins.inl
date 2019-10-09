namespace ae { namespace windows_sys {
	// *****************************************************************************************************
	// *****************************************************************************************************

	inline void C_NumberSpin::SetWheelBoost(ae::u32 boost) {
		m_Control.SetWheelBoost(boost);
	}
	inline ae::u32 C_NumberSpin::GetWheelBoost() const {
		return m_Control.GetWheelBoost();
	}

	inline void C_NumberSpin::LoopValue(bool bLoopValue) {
		m_Control.LoopValue(bLoopValue);
	}
	inline bool C_NumberSpin::IsValueLooped() const {
		return m_Control.IsValueLooped();
	}

	// *****************************************************************************************************
	// *****************************************************************************************************

	inline void C_FloatSpin::SetMinMax(float min, float max) {
		m_Control.SetMinMaxFloat(min, max);
	}
	inline float C_FloatSpin::GetMax() const {
		return m_Control.GetMaxFloat();
	}
	inline float C_FloatSpin::GetMin() const {
		return m_Control.GetMinFloat();
	}
	inline void C_FloatSpin::SetStep(float step) {
		m_Control.SetStepFloat(step);
	}
	inline float C_FloatSpin::GetStep() const {
		m_Control.GetStepFloat();
	}
	inline void C_FloatSpin::SetValue(float value) {
		m_Control.SetValueFloat(value);
	}
	inline float C_FloatSpin::GetValue() const {
		return m_Control.GetValueFloat();
	}

	inline void C_FloatSpin::SetFloatPrecision(int fltPrecision) {
		m_Control.SetFloatPrecision(fltPrecision);
	}
	inline int C_FloatSpin::GetFloatPrecision() const {
		return m_Control.GetFloatPrecision();
	}

	// *****************************************************************************************************
	// *****************************************************************************************************

	inline void C_IntegerSpin::SetMinMax(T_Integer min, T_Integer max) {
		m_Control.SetMinMaxInteger(min, max);
	}
	inline T_Integer C_IntegerSpin::GetMax() const {
		return m_Control.GetMaxInteger();
	}
	inline T_Integer C_IntegerSpin::GetMin() const {
		return m_Control.GetMinInteger();
	}
	inline void C_IntegerSpin::SetStep(T_Integer step) {
		m_Control.SetStepInteger(step);
	}
	inline T_Integer C_IntegerSpin::GetStep() const {
		m_Control.GetStepInteger();
	}
	inline void C_IntegerSpin::SetValue(T_Integer value) {
		m_Control.SetValueInteger(value);
	}
	inline T_Integer C_IntegerSpin::GetValue() const {
		return m_Control.GetValueInteger();
	}
} } // namespace ae { namespace windows_sys {
