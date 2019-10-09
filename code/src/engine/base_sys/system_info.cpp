#include "StdAfx.h"

#include "system_info.h"

namespace ae { namespace base {

C_SystemInfo::C_SystemInfo()
{
}


const char* C_SystemInfo::GetCPUName()
{
	return "?";
}

const char* C_SystemInfo::GetCPUBrand()
{
	return "?";
}

const char* C_SystemInfo::GetCPUType()
{
	return "?";
}

unsigned int C_SystemInfo::GetCPUFrequency()
{
	return 0;
}

unsigned int C_SystemInfo::GetCPUCores()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	return si.dwNumberOfProcessors;
}

const char* C_SystemInfo::GetOSName()
{
	return "?";
}

unsigned int C_SystemInfo::GetRAMTotal()
{
	return 0;
}

unsigned int C_SystemInfo::GetRAMAvailable()
{
	return 0;
}

C_SystemInfo& C_SystemInfo::Instance()
{
	static C_SystemInfo systemInfoInstance;
	return systemInfoInstance;
}

} } // namespace ae { namespace base {
