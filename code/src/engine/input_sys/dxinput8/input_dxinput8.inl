namespace ae { namespace input { namespace dxinput8 {

inline ae::math::float3 C_InputSys::MouseMove() const 
{
	return ae::math::float3(static_cast<float>(m_MouseState.lX), static_cast<float>(m_MouseState.lY), static_cast<float>(m_MouseState.lZ));
}

inline void C_InputSys::Freeze(bool bFreeze)
{
	m_bIsFrozen = bFreeze;
}

inline bool C_InputSys::IsFrozen() const
{
	return m_bIsFrozen;
}

} } } // namespace ae { namespace input { namespace dxinput8 {