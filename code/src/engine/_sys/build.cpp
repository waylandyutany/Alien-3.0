#include "StdAfx.h"

#include "build.h"

namespace ae {

const void C_BuildInfo::GetPlatformID(T_PlatformID& platformID)
{
#ifdef AE_PLATFORM_WIN32
	platformID[0] = 'w';
	platformID[1] = 'n';
	platformID[2] = '3';
	platformID[3] = '2';
#elif AE_PLATFORM_WIN64
	platformID[0] = 'w';
	platformID[1] = 'n';
	platformID[2] = '6';
	platformID[3] = '4';
#else
	#error No platform specified !
#endif
}

const char* C_BuildInfo::GetPlatformA()
{
#ifdef AE_PLATFORM_WIN32
	return "Win32";
#elif AE_PLATFORM_WIN64
	return "Win64";
#else
	#error No platform specified !
#endif
}

const wchar_t* C_BuildInfo::GetPlatformW()
{
#ifdef AE_PLATFORM_WIN32
	return L"Win32";
#elif AE_PLATFORM_WIN64
	return L"Win64";
#else
	#error No platform specified !
#endif
}

const char* C_BuildInfo::GetName()
{
#ifdef AE_PLATFORM_WIN32
	return "Alien Engine Win32";
#elif AE_PLATFORM_WIN64
	return "Alien Engine Win64";
#else
	#error No platform specified !
#endif
}

const wchar_t* C_BuildInfo::GetNameW()
{
#ifdef AE_PLATFORM_WIN32
	return L"Alien Engine Win32";
#elif AE_PLATFORM_WIN64
	return L"Alien Engine Win64";
#else
	#error No platform specified !
#endif
}

ae::u32 C_BuildInfo::Version()
{
	return 3;
}

ae::u32 C_BuildInfo::SubVersion()
{
	return 0;
}

} // namespace ae {

