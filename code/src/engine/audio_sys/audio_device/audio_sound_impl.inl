namespace ae { namespace audio {
// *****************************************************************************************************
inline void C_SoundImpl::Play(bool looped)
{
	m_Flags = 0;
	m_Flags |= cIsPlaying;
	if(looped) m_Flags |= cIsLooped;
	m_UpdateSamplePosFunc = looped ? &C_SoundImpl::UpdateSamplePosLooped : &C_SoundImpl::UpdateSamplePosNoLooped;
}

inline void C_SoundImpl::Stop()
{
	m_Flags &= ~cIsPlaying;
	m_UpdateSamplePosFunc = &C_SoundImpl::UpdateSamplePosStopped;
}

inline bool C_SoundImpl::IsPlaying() const
{
	return (m_Flags & cIsPlaying) ? true : false;
}

inline bool C_SoundImpl::IsLooped() const
{
	return (m_Flags & cIsLooped) ? true : false;
}
// *****************************************************************************************************
inline void C_SoundImpl::SetVolume(float volume)
{
	m_fVolume = volume;
}

inline float C_SoundImpl::GetVolume() const
{
	return m_fVolume;
}
// *****************************************************************************************************
inline void C_SoundImpl::SetFrequency(float frequency)
{
	m_fFrequency = frequency;
}

inline float C_SoundImpl::GetFrequency() const
{
	return m_fFrequency;
}
// *****************************************************************************************************
	
inline void C_SoundImpl::SetPosition(const ae::math::float3& position)
{
	m_RenderParams.position = position;
}

inline const ae::math::float3& C_SoundImpl::GetPosition() const
{
	return m_RenderParams.position;
}
// *****************************************************************************************************
inline const S_SoundRenderParams& C_SoundImpl::GetSoundRenderParams() const
{
	return m_RenderParams;
}

inline const S_AudioDataInfo& C_SoundImpl::GetAudioDataInfo() const
{
	return m_AudioDataInfo;
}
// *****************************************************************************************************
inline void C_SoundImpl::SetVelocity(const ae::math::float3& velocity)
{
	m_RenderParams.velocity = velocity;
}

inline const ae::math::float3& C_SoundImpl::GetVelocity() const
{
	return m_RenderParams.velocity;
}
// *****************************************************************************************************
inline void C_SoundImpl::SetOrientation(const ae::math::float3& front, const ae::math::float3& up)
{
	m_RenderParams.orientationFront = front;
	m_RenderParams.orientationUp = up;
}

inline void C_SoundImpl::GetOrientation(ae::math::float3& front, ae::math::float3& up) const
{
	front = m_RenderParams.orientationFront;
	up = m_RenderParams.orientationUp;
}
// *****************************************************************************************************
inline void C_SoundImpl::SetLowPassFrequency(float frequency)
{
	m_RenderParams.fLowPassFrequency = frequency;
}

inline float C_SoundImpl::GetLowPassFrequency() const
{
	return m_RenderParams.fLowPassFrequency;
}
// *****************************************************************************************************
inline void C_SoundImpl::SeekSamplePos(ae::u32 samplePos)
{
	m_SamplePos = samplePos % m_AudioDataInfo.numSamples;
	m_Flags |= cIsSeeked;
}

inline ae::u32 C_SoundImpl::GetSamplePos() const
{
	return m_SamplePos;
}

inline bool C_SoundImpl::IsSeeked() const
{
	return m_Flags & cIsSeeked ? true : false;
}

inline void C_SoundImpl::ClearSeek()
{
	m_Flags &= ~cIsSeeked;
}
// *****************************************************************************************************

} } // namespace ae { namespace audio {
