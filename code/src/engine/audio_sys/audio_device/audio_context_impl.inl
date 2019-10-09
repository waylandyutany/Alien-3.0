namespace ae { namespace audio {

inline void C_ContextImpl::SoundDeleted()
{
	m_bSoundDeleted = true;
}

inline void C_ContextImpl::SetRenderSoundsLimits(ae::u32 priorityRenderSounds, ae::u32 totalRenderSounds)
{
	m_PriorityRenderSounds = priorityRenderSounds;
	m_TotalRenderSounds = totalRenderSounds;
}

inline void C_ContextImpl::GetRenderSoundsLimits(ae::u32& priorityRenderSounds, ae::u32& totalRenderSounds) const
{
	priorityRenderSounds = m_PriorityRenderSounds;
	totalRenderSounds = m_TotalRenderSounds;
}

inline void C_ContextImpl::SetVolume(float volume)
{
	m_RenderParams.fVolume = volume;
}

inline float C_ContextImpl::GetVolume() const
{
	return m_RenderParams.fVolume;
}

inline const S_ContextRenderParams& C_ContextImpl::GetRenderParams() const
{
	return m_RenderParams;
}

inline bool C_ContextImpl::IsPaused() const { return m_bPaused;}
inline void C_ContextImpl::Pause() { m_bPaused = true; }
inline void C_ContextImpl::Resume() { m_bPaused = false; }

} } // namespace ae { namespace audio {

