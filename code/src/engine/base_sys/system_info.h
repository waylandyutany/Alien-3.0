#pragma once

#ifndef AE_BASE_SYS_SYSTEM_INFO_H
#define AE_BASE_SYS_SYSTEM_INFO_H

namespace ae { namespace base {

class C_SystemInfo
{
	C_SystemInfo();
public:
	const char* GetCPUName();
	const char* GetCPUBrand();
	const char* GetCPUType();
	unsigned int GetCPUFrequency();
	unsigned int GetCPUCores();

	const char* GetOSName();

	unsigned int GetRAMTotal();
	unsigned int GetRAMAvailable();

	static C_SystemInfo& Instance();
};

} } // namespace ae { namespace base {

#endif // #ifdef AE_BASE_SYS_SYSTEM_INFO_H
