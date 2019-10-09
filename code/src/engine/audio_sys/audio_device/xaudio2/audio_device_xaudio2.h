#pragma once

#ifndef AE_AUDIO_SYS_DEVICE_XAUDIO2_H
#define AE_AUDIO_SYS_DEVICE_XAUDIO2_H

#include "../audio_sound_impl.h"
#include "../audio_sound_render_request.h"

namespace ae { namespace audio { 
	struct S_Init;
	class C_AudioDevice;
} } // namespace ae { namespace audio { 

namespace ae { namespace audio { namespace xaudio2 {

	class C_SoundRender;

	class C_AudioDevice
	{
		NOCOPY_CLASS(C_AudioDevice);
		friend ae::audio::C_AudioDevice;

		C_AudioDevice(const S_Init&);
		~C_AudioDevice();

		IXAudio2* m_XAudio2;
	    X3DAUDIO_HANDLE m_X3DInstance;
	    IXAudio2MasteringVoice* m_MasteringVoice;
		XAUDIO2_DEVICE_DETAILS m_DeviceDetails;

		typedef std::vector<C_SoundRender*> T_SoundRenders;
		T_SoundRenders m_3DSoundRenders;
		T_SoundRenders m_2DSoundRenders;

		typedef std::vector<IUnknown*> T_Reverbs;
		T_Reverbs m_Reverbs;

		void Render(T_SoundRenderRequests& renderRequests, ae::u32 activeSoundFrame);

		void ReleaseUnactiveSoundRenders(T_SoundRenders& soundRenders, ae::u32 activeSoundFrame);
		void CreateNewRenderAssociations(T_SoundRenderRequests& renderRequests);
		void UpdateRenderParams(T_SoundRenders& soundRenders);
		C_SoundRender* GetFreeSoundRender(bool is3D);
	};

} } } // namespace ae { namespace audio { namespace xaudio2 {

#endif // #ifdef AE_AUDIO_SYS_DEVICE_XAUDIO2_H
