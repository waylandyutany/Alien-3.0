namespace ae { namespace audio {

// *****************************************************************************************************
inline void C_Context::SetVolume(float volume)
{
	m_ContextImpl->SetVolume(volume);
}

inline float C_Context::GetVolume() const
{
	return m_ContextImpl->GetVolume();
}
// *****************************************************************************************************
inline bool C_Context::IsPaused() const
{
	return m_ContextImpl->IsPaused();
}

inline void C_Context::Pause()
{
	m_ContextImpl->Pause();
}

inline void C_Context::Resume()
{
	m_ContextImpl->Resume();
}
// *****************************************************************************************************

} } // namespace ae { namespace audio {
