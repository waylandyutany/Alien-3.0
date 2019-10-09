#pragma once

#ifndef AE_AUDIO_SYS_DEVICE_H
#define AE_AUDIO_SYS_DEVICE_H

#ifdef AE_AUDIO_XAUDIO2
#include "xaudio2/audio_device_xaudio2.h"
#else
	#error No audio resources implementation available !
#endif // #ifdef AE_AUDIO_XAUDIO2

#include "audio_sound_impl.h"

namespace ae { namespace audio {

	class C_AudioSys;
	class C_AudioScene;
	struct S_Init;

	class C_AudioDevice
	{
		C_AudioDevice();
		NOCOPY_CLASS(C_AudioDevice);
#ifdef AE_AUDIO_XAUDIO2
		ae::audio::xaudio2::C_AudioDevice m_Impl;
#endif // #ifdef AE_AUDIO_XAUDIO2
		C_AudioDevice(const S_Init&);
		~C_AudioDevice();

		friend class C_AudioSys;
		friend class C_AudioScene;

		void Render(T_SoundRenderRequests& renderRequests, ae::u32 activeSoundFrame);
	};

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_SYS_DEVICE_H
