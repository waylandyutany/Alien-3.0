namespace ae { namespace input {

inline bool C_InputSys::KeyState(E_Key_State keyState, E_Key_Code keyCode)
{
	return m_Impl.KeyState(keyState, keyCode);
}

inline bool C_InputSys::MouseState(E_Key_State keyState, E_Mouse_Button mouseButton)
{
	return m_Impl.MouseState(keyState, mouseButton);
}

inline ae::math::float3 C_InputSys::MouseMove() const {
	return m_Impl.MouseMove();
}

inline ae::math::float2 C_InputSys::MouseCursorPos() const {
	return m_Impl.MouseCursorPos();
}

inline bool C_InputSys::HandleMsgPump()
{
	return m_Impl.HandleMsgPump();
}

inline void C_InputSys::Update()
{
	m_Impl.Update();
}

inline void C_InputSys::Freeze(bool bFreeze)
{
	m_Impl.Freeze(bFreeze);
}

inline bool C_InputSys::IsFrozen() const
{
	return m_Impl.IsFrozen();
}

inline void C_InputSys::Quit()
{
	m_Impl.Quit();
}

} } // namespace ae { namespace input {
