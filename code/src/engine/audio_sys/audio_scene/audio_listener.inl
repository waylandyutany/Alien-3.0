namespace ae { namespace audio {

inline void C_Listener::SetPosition(const ae::math::float3& position)
{
	m_Impl->SetPosition(position);
}

inline const ae::math::float3& C_Listener::GetPosition() const
{
	return m_Impl->GetPosition();
}

inline void C_Listener::SetVolume(float volume)
{
	m_Impl->SetVolume(volume);
}

inline float C_Listener::GetVolume() const
{
	return m_Impl->GetVolume();
}

inline void C_Listener::SetVelocity(const ae::math::float3& velocity)
{
	m_Impl->SetVelocity(velocity);
}

inline const ae::math::float3& C_Listener::GetVelocity() const
{
	return m_Impl->GetVelocity();
}

inline void C_Listener::SetOrientation(const ae::math::float3& front, const ae::math::float3& up)
{
	m_Impl->SetOrientation(front,up);
}

inline void C_Listener::GetOrientation(ae::math::float3& front, ae::math::float3& up) const
{
	m_Impl->GetOrientation(front,up);
}

} } // namespace ae { namespace audio {
