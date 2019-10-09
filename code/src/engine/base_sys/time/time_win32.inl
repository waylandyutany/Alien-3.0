namespace ae { namespace base {

inline ae::u64 C_Time::GetTimeMicroseconds() const
{
	ae::u64 time = GetCounter() - m_StartTime;
	return time / m_FreqDiv1000000;	
}

inline float C_Time::GetTimeSeconds() const
{
	ae::u64 time = GetCounter() - m_StartTime;
	return (float)(m_FreqInv*((double)time));
}

inline ae::u64 C_Time::GetCounter() const
{
	{ return __rdtsc(); }
}

inline void SecondsToHMS(float seconds, ae::u32& h, ae::u32& m, ae::u32& s)
{
	h = static_cast<ae::u32>(seconds / 3600.0f);
	m = static_cast<ae::u32>((seconds / 60.0f) - h * 60.0f);
	s = static_cast<ae::u32>(seconds - (h * 3600.0f) - (m * 60.0f));
}

} } // namespace ae { namespace base {
