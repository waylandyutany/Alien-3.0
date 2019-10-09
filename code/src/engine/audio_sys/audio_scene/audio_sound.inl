namespace ae { namespace audio {

inline void C_Sound::Play(bool looped)
{
	m_SoundImpl->Play(looped);
}

inline void C_Sound::Stop()
{
	m_SoundImpl->Stop();
}

inline bool C_Sound::IsPlaying() const
{
	return m_SoundImpl->IsPlaying();
}

inline void C_Sound::SetVolume(float volume)
{
	m_SoundImpl->SetVolume(volume);
}

inline float C_Sound::GetVolume() const
{
	return m_SoundImpl->GetVolume();
}

inline void C_Sound::SetFrequency(float frequency)
{
	m_SoundImpl->SetFrequency(frequency);
}

inline float C_Sound::GetFrequency() const
{
	return m_SoundImpl->GetFrequency();
}

inline void C_Sound::SetPosition(const ae::math::float3& position)
{
	m_SoundImpl->SetPosition(position);
}

inline const ae::math::float3& C_Sound::GetPosition() const
{
	return m_SoundImpl->GetPosition();
}

inline void C_Sound::SetVelocity(const ae::math::float3& velocity)
{
	m_SoundImpl->SetVelocity(velocity);
}

inline const ae::math::float3& C_Sound::GetVelocity() const
{
	return m_SoundImpl->GetVelocity();
}

inline void C_Sound::SetOrientation(const ae::math::float3& front, const ae::math::float3& up)
{
	m_SoundImpl->SetOrientation(front,up);
}

inline void C_Sound::GetOrientation(ae::math::float3& front, ae::math::float3& up) const
{
	m_SoundImpl->GetOrientation(front,up);
}

inline void C_Sound::SetLowPassFrequency(float frequency)
{
	m_SoundImpl->SetLowPassFrequency(frequency);
}

inline float C_Sound::GetLowPassFrequency() const
{
	return m_SoundImpl->GetLowPassFrequency();
}

inline void C_Sound::SeekSamplePos(ae::u32 samplePos)
{
	m_SoundImpl->SeekSamplePos(samplePos);
}

inline ae::u32 C_Sound::GetSamplePos() const
{
	return m_SoundImpl->GetSamplePos();
}

} } // namespace ae { namespace audio {
