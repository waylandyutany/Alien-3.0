#include "StdAfx.h"

#include "time.h"

namespace ae { namespace base {

C_Time::C_Time()
{
	// force to run on one processor to be sure rdtsc measurment is correct
	SetThreadAffinityMask(GetCurrentThread(),1);

	m_Freq = 0;
	LARGE_INTEGER freqQPC;
	QueryPerformanceFrequency(&freqQPC);

	for (ae::u32 i = 0;i < 4;++i)
	{
		ae::u64 begin = GetCounter();
		LARGE_INTEGER beginQpc;			
		QueryPerformanceCounter(&beginQpc);

		Sleep(10);

		ae::u64 end = GetCounter();
		LARGE_INTEGER endQpc;			
		QueryPerformanceCounter(&endQpc);

		ae::u64 len = end - begin;
		LONGLONG lenQPC = endQpc.QuadPart - beginQpc.QuadPart;			
		double lenQPCsec = ((double)lenQPC)/(double)freqQPC.QuadPart;

		ae::u64 freq = (ae::u64)(((double)len)/lenQPCsec);
			
		if (freq>m_Freq)
				m_Freq = freq;
	}

	m_StartTime = GetCounter();

	m_FreqDiv1000000 = m_Freq/1000000;
	m_FreqDiv1000 = m_Freq/1000;		
	m_FreqFlt = double(m_Freq);
	m_FreqDiv1e6 = m_FreqFlt/1000000.0;
	m_FreqInv = 1.0/m_FreqFlt;
}

C_Time time;

} } // namespace ae { namespace base {
