#pragma once

#ifndef AE_AUDIO_SYS_SOUND_RENDER_XAUDIO2_H
#define AE_AUDIO_SYS_SOUND_RENDER_XAUDIO2_H

#include "../audio_data_stream_notify.h"

namespace ae { namespace audio { 
	class C_AudioDevice;
	class C_AudioDataStream;
	class C_SoundImpl;
} } // namespace ae { namespace audio { 

namespace ae { namespace audio { namespace xaudio2 {

	class C_AudioStreamsDecoder;

	struct S_SoundRenderUpdateParams
	{
		S_SoundRenderUpdateParams(XAUDIO2_DEVICE_DETAILS& _deviceDetails, X3DAUDIO_HANDLE& _audioX3D, IXAudio2MasteringVoice* _masteringVoice) :
			deviceDetails(_deviceDetails),
			audioX3D(_audioX3D),
			masteringVoice(_masteringVoice) {}
		X3DAUDIO_HANDLE& audioX3D;
	    IXAudio2MasteringVoice* masteringVoice;
		XAUDIO2_DEVICE_DETAILS& deviceDetails;
	private:
		NOCOPY_CLASS(S_SoundRenderUpdateParams);
	};

	class C_SoundRender : public I_AudioDataStreamNotify
	{
		NOCOPY_CLASS(C_SoundRender);

		C_SoundImpl* soundImpl;
		C_AudioDataStream* audioDataStream;
		IXAudio2SourceVoice* sourceVoice;

		char* buffers[2];
		size_t bufferSize;
		size_t bufferUsageIndex;
		bool bLastBuffer;
		float* pMatrixCoefficients;
// *****************************************************************************************************
		void SeekSamples(ae::u32 samplePos);

		void FillAndCommitBufferLooped(char* buffer, size_t numSamples);
		void FillAndCommitBufferNoLooped(char* buffer, size_t numSamples);
		void CommitBuffer(char* buffer, size_t size, bool bLastBuffer);

		typedef void (C_SoundRender::*T_FillAndCommitBuffer)(char* buffer, size_t numSamples);
		T_FillAndCommitBuffer m_FillAndCommitBufferFunc;
// *****************************************************************************************************
		virtual void OnStreamDelete();
	public:
		C_SoundRender(XAUDIO2_DEVICE_DETAILS&, IXAudio2*, char* buffer0, char* buffer1, size_t _bufferSize, WAVEFORMATEX& wfex);
		~C_SoundRender();
// *****************************************************************************************************
		void Acquire(C_SoundImpl* _soundImpl, C_AudioDataStream* _stream, const S_SoundRenderUpdateParams&);
		void Unacquire();
		bool IsAcquired() const;
// *****************************************************************************************************
		void OnBufferEnd();
		void WriteBuffer();
// *****************************************************************************************************
		void UpdateParams(const S_SoundRenderUpdateParams&);
// *****************************************************************************************************
		C_SoundImpl* SoundImpl();
		C_AudioDataStream* AudioStream();
	};

	inline C_SoundImpl* C_SoundRender::SoundImpl() { return soundImpl;	}
	inline C_AudioDataStream* C_SoundRender::AudioStream() { return audioDataStream; }
	inline bool C_SoundRender::IsAcquired() const { return audioDataStream ? true : false; }

} } } // namespace ae { namespace audio { namespace xaudio2 {

#endif // #ifdef AE_AUDIO_SYS_SOUND_RENDER_XAUDIO2_H
