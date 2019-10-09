#include "StdAfx.h"

#include "voice_callback_xaudio2.h"
#include "sound_render_xaudio2.h"

#include "../audio_data_stream.h"
#include "../../audio_codecs/i_audio_codec.h"

namespace ae { namespace audio { namespace xaudio2 {

void C_VoiceCallback::OnBufferEnd( void* pBufferContext )
{
	C_SoundRender* pSoundRender(static_cast<C_SoundRender*>(pBufferContext));
	if(pSoundRender->IsAcquired())
		pSoundRender->OnBufferEnd();
}

void C_VoiceCallback::OnBufferStart( void* /*pBufferContext*/ )
{
}

void C_VoiceCallback::OnLoopEnd( void* /*pBufferContext*/ )
{
}

void C_VoiceCallback::OnStreamEnd()
{
}

void C_VoiceCallback::OnVoiceError( void* /*pBufferContext*/, HRESULT /*Error*/ )
{
}

void C_VoiceCallback::OnVoiceProcessingPassEnd()
{
}

void C_VoiceCallback::OnVoiceProcessingPassStart( UINT32 /*BytesRequired*/ )
{
}

C_VoiceCallback::C_VoiceCallback()
{
}

C_VoiceCallback::~C_VoiceCallback()
{
}

} } } // namespace ae { namespace audio { namespace xaudio2 {
