#include "StdAfx.h"

#include "sound_render_xaudio2.h"

#include "../audio_data_stream.h"
#include "../../audio_codecs/i_audio_codec.h"

#include "../audio_sound_impl.h"
#include "../listener_render_params.h"

#include "voice_callback_xaudio2.h"

namespace ae { namespace audio { namespace xaudio2 {

	static C_VoiceCallback g_VoiceCallback;
// *****************************************************************************************************
// *****************************************************************************************************
	C_SoundRender::C_SoundRender(XAUDIO2_DEVICE_DETAILS& deviceDetails, IXAudio2* pXAudio2, char* buffer0, char* buffer1, size_t _bufferSize, WAVEFORMATEX& wfex) :
		bufferSize(_bufferSize),
		bufferUsageIndex(0),
		soundImpl(0),
		sourceVoice(0),
		audioDataStream(0),
		pMatrixCoefficients(new float[deviceDetails.OutputFormat.Format.nChannels * wfex.nChannels])
	{
		buffers[0] = buffer0;
		buffers[1] = buffer1;

		HRESULT result;
		result = pXAudio2->CreateSourceVoice( &sourceVoice, &wfex, XAUDIO2_VOICE_USEFILTER, XAUDIO2_DEFAULT_FREQ_RATIO, &g_VoiceCallback );
	}

	C_SoundRender::~C_SoundRender()
	{
		sourceVoice->DestroyVoice();
		delete [] pMatrixCoefficients;
	}
// *****************************************************************************************************
// *****************************************************************************************************
	void C_SoundRender::UpdateParams(const S_SoundRenderUpdateParams& prm)
	{
// *****************************************************************************************************
// Do seek if set so
		if(soundImpl->IsSeeked())
		{
			SeekSamples(soundImpl->GetSamplePos());
			soundImpl->ClearSeek();
		}
// *****************************************************************************************************
		const S_SoundRenderParams& renderParams(soundImpl->GetSoundRenderParams());
		const S_ListenerRenderParams& listenerParams(*renderParams.listenerParams);
		const S_AudioDataInfo& audioDataInfo(soundImpl->GetAudioDataInfo());

		sourceVoice->SetVolume(renderParams.fVolume);
		sourceVoice->SetFrequencyRatio(renderParams.fFrequency);

		XAUDIO2_FILTER_PARAMETERS filterParams;
		filterParams.Frequency = XAudio2CutoffFrequencyToRadians(renderParams.fLowPassFrequency, 48000);    
		filterParams.OneOverQ = 1.0f;
		filterParams.Type = LowPassFilter;
		sourceVoice->SetFilterParameters(&filterParams);

		if(audioDataInfo.numChannels == 1)
		{
			X3DAUDIO_LISTENER audioListener = {0};
			X3DAUDIO_EMITTER audioEmitter = {0};
			X3DAUDIO_DSP_SETTINGS dspSettings = {0};

			audioListener.OrientFront = D3DXVECTOR3(listenerParams.orientationFront.x, listenerParams.orientationFront.y, listenerParams.orientationFront.z);
			audioListener.OrientTop = D3DXVECTOR3(listenerParams.orientationUp.x, listenerParams.orientationUp.y, listenerParams.orientationUp.z);

			audioEmitter.OrientFront = D3DXVECTOR3( 0, 0, 1 );
			audioEmitter.OrientTop = D3DXVECTOR3( 0, 1, 0 );
			audioEmitter.Position = D3DXVECTOR3( renderParams.direction.x, 0, renderParams.direction.z);
			audioEmitter.CurveDistanceScaler = 1.0;
			audioEmitter.ChannelCount = 1;
//		    audioEmitter.pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;
		    //audioEmitter.ChannelRadius = 16.0f;

			const ae::u32 inputChannels = 1;
			const ae::u32 outputChannels = prm.deviceDetails.OutputFormat.Format.nChannels;

			dspSettings.SrcChannelCount = inputChannels;
			dspSettings.DstChannelCount = outputChannels;

			dspSettings.pMatrixCoefficients = pMatrixCoefficients;

			ae::u32 flags(X3DAUDIO_CALCULATE_MATRIX);
			X3DAudioCalculate(prm.audioX3D, &audioListener, &audioEmitter, flags, &dspSettings);
	
			HRESULT result;
            result = sourceVoice->SetOutputMatrix( prm.masteringVoice, inputChannels, outputChannels,
                                    dspSettings.pMatrixCoefficients );
		}
	}
// *****************************************************************************************************
// *****************************************************************************************************
	void C_SoundRender::OnBufferEnd()
	{
		if(bLastBuffer)
		{
			sourceVoice->Stop();
			sourceVoice->FlushSourceBuffers();
			soundImpl->Stop();
		}
		else WriteBuffer();
	}
// *****************************************************************************************************
// *****************************************************************************************************
	void C_SoundRender::FillAndCommitBufferLooped(char* buffer, size_t numSamples)
	{
		const S_AudioDataInfo& audioDataInfo(soundImpl->GetAudioDataInfo());

		size_t filledSamples = 0;
		while(filledSamples < numSamples)
		{
			size_t readedSamples = 0;
			size_t samplesToRead(numSamples - filledSamples);

			audioDataStream->GetCodec().ReadSamples(buffer + audioDataInfo.SamplesToBytes(filledSamples), samplesToRead, readedSamples);

			filledSamples += readedSamples;

			if(readedSamples < samplesToRead)
				audioDataStream->GetCodec().Seek(0);
		}

		CommitBuffer(buffer, audioDataInfo.SamplesToBytes(numSamples), false);
	}

	void C_SoundRender::FillAndCommitBufferNoLooped(char* buffer, size_t numSamples)
	{
		const S_AudioDataInfo& audioDataInfo(soundImpl->GetAudioDataInfo());
		size_t readedSamples = 0;
		audioDataStream->GetCodec().ReadSamples(buffer, numSamples, readedSamples);
		CommitBuffer(buffer, audioDataInfo.SamplesToBytes(readedSamples), readedSamples != numSamples ? true : false);
	}

	void C_SoundRender::CommitBuffer(char* buffer, size_t size, bool bLastBuffer)
	{
		XAUDIO2_BUFFER audioBuffer = {0};
		audioBuffer.pAudioData = (BYTE*)buffer;
		audioBuffer.pContext = reinterpret_cast<void*>(this);
		audioBuffer.AudioBytes = (UINT32)size;
		audioBuffer.Flags = bLastBuffer ? XAUDIO2_END_OF_STREAM : 0;
		sourceVoice->SubmitSourceBuffer( &audioBuffer );
		this->bLastBuffer = bLastBuffer;
	}

	void C_SoundRender::WriteBuffer()
	{
		const S_AudioDataInfo& audioDataInfo(soundImpl->GetAudioDataInfo());

		// zeroing sound buffer
		memset(buffers[bufferUsageIndex], 0, bufferSize);
		// Continue only if valid audioDataStream
		if(!audioDataStream) return;

		(this->*m_FillAndCommitBufferFunc)(buffers[bufferUsageIndex], audioDataInfo.BytesToSamples(bufferSize));

		bufferUsageIndex++;
		bufferUsageIndex = bufferUsageIndex % 2;
	}
// *****************************************************************************************************
// *****************************************************************************************************
	void C_SoundRender::SeekSamples(ae::u32 samplePos)
	{
		if(!audioDataStream) return;


		sourceVoice->Stop();
		sourceVoice->FlushSourceBuffers();
		audioDataStream->GetCodec().Seek(samplePos);
		bufferUsageIndex = 0;
		WriteBuffer();
		WriteBuffer();
		sourceVoice->Start();
	}

// *****************************************************************************************************
// *****************************************************************************************************
	void C_SoundRender::Unacquire()
	{
		sourceVoice->Stop();
		sourceVoice->FlushSourceBuffers();
		soundImpl->SetRenderLink(0);
		soundImpl = 0;
		audioDataStream = 0;
	}
// *****************************************************************************************************
	void C_SoundRender::Acquire(C_SoundImpl* _soundImpl, C_AudioDataStream* _stream, const S_SoundRenderUpdateParams& prm)
	{
		soundImpl = _soundImpl;
		audioDataStream = _stream;
		bufferUsageIndex = 0;
		bLastBuffer = false;

		const S_AudioDataInfo& audioDataInfo(soundImpl->GetAudioDataInfo());

		sourceVoice->SetSourceSampleRate((UINT32)audioDataInfo.sampleRate);
		soundImpl->SetRenderLink(this);

		m_FillAndCommitBufferFunc = soundImpl->IsLooped() ? &C_SoundRender::FillAndCommitBufferLooped : &C_SoundRender::FillAndCommitBufferNoLooped;

		audioDataStream->GetCodec().Seek(_soundImpl->GetSamplePos());
		_soundImpl->ClearSeek();

		WriteBuffer();
		WriteBuffer();
		UpdateParams(prm);

		sourceVoice->Start();
	}
// *****************************************************************************************************
// *****************************************************************************************************
	void C_SoundRender::OnStreamDelete()
	{
		Unacquire();
	}
// *****************************************************************************************************
} } } // namespace ae { namespace audio { namespace xaudio2 {
