#pragma once

#ifndef AE_BASE_SYS_TIME_H
#define AE_BASE_SYS_TIME_H

namespace ae { namespace base {

	class C_Time
	{
		NOCOPY_CLASS(C_Time);

		ae::u64 m_Freq;
		ae::u64 m_StartTime;
		ae::u64 m_FreqDiv1000000;
		ae::u64 m_FreqDiv1000;		
		double m_FreqFlt;	
		double m_FreqInv;	
		double m_FreqDiv1e6;

		ae::u64 GetCounter() const;
	public:
		C_Time();
		ae::u64 GetTimeMicroseconds() const;
		float GetTimeSeconds() const;
	};

	extern C_Time time;

	void SecondsToHMS(float seconds, ae::u32& h, ae::u32& m, ae::u32& s);
} } // namespace ae { namespace base {

#include "time_win32.inl"

#endif // #ifdef AE_BASE_SYS_TIME_H
