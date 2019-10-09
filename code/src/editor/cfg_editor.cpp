#include "stdafx.h"

#if defined(AE_EDITOR)

#include "cfg_editor.h"

namespace ae { namespace editor {

char* C_BuildInfo::GetName()
{
#ifdef AE_PLATFORM_WIN32
	return "Alien Editor Win32";
#elif AE_PLATFORM_WIN64
	return "Alien Editor Win64";
#else
	#error No platform specified !
#endif
}

wchar_t* C_BuildInfo::GetNameW()
{
#ifdef AE_PLATFORM_WIN32
	return L"Alien Editor Win32";
#elif AE_PLATFORM_WIN64
	return L"Alien Editor Win64";
#else
	#error No platform specified !
#endif
}

} } //namespace ae { namespace editor {

#endif // #if defined(AE_EDITOR)
