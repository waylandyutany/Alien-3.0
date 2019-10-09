#include "StdAfx.h"

#include "audio_sound.h"

#include "../audio_scene/audio_scene.h"
#include "../audio_sys.h"

#include "../../debug_sys/output/outputs.h"

namespace ae { namespace audio {

C_Sound::C_Sound( T_SoundPtr& soundPtr, T_ContextPtr& contextPtr ) : 
	m_SoundImpl(soundPtr),
	m_ContextImpl(contextPtr)
{
}

C_Sound::~C_Sound()
{
	m_SoundImpl->SetDead();
	m_ContextImpl->SoundDeleted();
	C_AudioSys::Instance().Scene().AudioObjectDeleted();
}

} } // namespace ae { namespace audio {
