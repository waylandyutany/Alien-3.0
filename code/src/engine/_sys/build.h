#pragma once
#ifndef AE_BUILD_H
#define AE_BUILD_H

// This header file is automatically included to every alien's project (editor, game, engine)

#include "types.h"
#include "assert.h"
#include "build_check.h"

#pragma warning( disable : 4355 ) // 'this' : used in base member initializer list
#pragma warning( disable : 4100 ) // unreferenced formal parameter

#define AE_PHYSX
#define AE_RENDER_DX11

#define AE_INPUT_DXINPUT8

#define AE_AUDIO_XAUDIO2
#define AE_AUDIO_OGG_CODEC

#ifdef AE_PLATFORM_WIN32
//	#define AE_AUDIO_FLAC_CODEC
	#define AE_32BIT
#endif // #ifdef AE_PLATFORM_WIN32

#ifdef AE_PLATFORM_WIN64
	#define AE_64BIT
#endif // #ifdef AE_PLATFORM_WIN32

#define AE_AUDIO_WAV_CODEC

#define AE_EDITOR

#define NOCOPY_CLASS(className) \
	const className& operator = (const className&); \
	className(const className&);

#define REFERENCE(p) (p)

namespace ae {

	static const size_t maxStringSize	= 512;
	static const size_t maxFilePathSize = 1024;
	typedef char T_PlatformID[4];

	class C_BuildInfo
	{
	public:
		static const char* GetName();
		static const wchar_t* GetNameW();

		static const char* GetPlatformA();
		static const wchar_t* GetPlatformW();

		static const void GetPlatformID(T_PlatformID& platformID);

		static ae::u32 Version();
		static ae::u32 SubVersion();
	};
} // namespace ae {

#endif // #ifdef AE_BUILD_H
