#pragma once

#ifndef AE_AUDIO_SYS_VOICE_CALLBACK_XAUDIO2_H
#define AE_AUDIO_SYS_VOICE_CALLBACK_XAUDIO2_H

namespace ae { namespace audio { namespace xaudio2 {

	class C_VoiceCallback : public IXAudio2VoiceCallback
	{
		virtual void __stdcall OnBufferEnd( void *pBufferContext );
		virtual void __stdcall OnBufferStart( void *pBufferContext );
		virtual void __stdcall OnLoopEnd( void *pBufferContext );
		virtual void __stdcall OnStreamEnd();
		virtual void __stdcall OnVoiceError( void *pBufferContext, HRESULT Error );
		virtual void __stdcall OnVoiceProcessingPassEnd();
		virtual void __stdcall OnVoiceProcessingPassStart( UINT32 BytesRequired );
	public:
		C_VoiceCallback();
		virtual ~C_VoiceCallback();
	};

} } } // namespace ae { namespace audio { namespace xaudio2 {

#endif // #ifdef AE_AUDIO_SYS_VOICE_CALLBACK_XAUDIO2_H
