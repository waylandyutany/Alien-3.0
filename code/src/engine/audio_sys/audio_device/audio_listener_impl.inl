namespace ae { namespace audio {

inline void C_ListenerImpl::SetPosition(const ae::math::float3& position)
{
	m_RenderParams.position = position;
}

inline const ae::math::float3& C_ListenerImpl::GetPosition() const
{
	return m_RenderParams.position;
}

inline void C_ListenerImpl::SetVolume(float volume)
{
	m_RenderParams.fVolume = volume;
}

inline float C_ListenerImpl::GetVolume() const
{
	return m_RenderParams.fVolume;
}

inline void C_ListenerImpl::SetVelocity(const ae::math::float3& velocity)
{
	m_RenderParams.velocity = velocity;
}

inline void C_ListenerImpl::SetOrientation(const ae::math::float3& front, const ae::math::float3& up)
{
	m_RenderParams.orientationFront = front;
	m_RenderParams.orientationUp = up;
}

inline const ae::math::float3& C_ListenerImpl::GetVelocity() const
{
	return m_RenderParams.velocity;
}

inline void C_ListenerImpl::GetOrientation(ae::math::float3& front, ae::math::float3& up) const
{
	front = m_RenderParams.orientationFront;
	up = m_RenderParams.orientationUp;
}

inline const S_ListenerRenderParams& C_ListenerImpl::GetRenderParams() const
{
	return m_RenderParams;
}

} } // namespace ae { namespace audio {

