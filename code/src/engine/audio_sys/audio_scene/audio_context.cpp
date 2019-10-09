#include "StdAfx.h"

#include "audio_context.h"
#include "audio_sound.h"

#include "../audio_device/audio_sound_impl.h"
#include "../audio_scene/audio_scene.h"
#include "../audio_sys.h"

#include "audio_listener.h"

#include "../../debug_sys/output/outputs.h"
#include "../../base_sys/hash.h"

namespace ae { namespace audio {

C_Context::C_Context(T_ContextPtr& contextImpl) : m_ContextImpl(contextImpl)
{
}

C_Context::~C_Context()
{
	m_ContextImpl->SetDead();
	C_AudioSys::Instance().Scene().AudioObjectDeleted();
}

C_Sound* C_Context::Create3DSound(const char* audioDataID)
{
	T_SoundPtr soundPtr(new C_3DSoundImpl(ae::base::CalculateHashI32(audioDataID)));
	const S_AudioDataInfo& adi(soundPtr->GetAudioDataInfo());
	if(adi.numChannels == 0)
		DBG_ERR << "3D sound has invalid audio data '" << audioDataID << "' !" << std::endl;
	if(adi.numChannels != 1)
		DBG_ERR << "3D sound must have mono audio data '" << audioDataID << "' !" << std::endl;
	else m_ContextImpl->m_Sounds.push_back(soundPtr);

	return new C_Sound(soundPtr, m_ContextImpl);
}

C_Sound* C_Context::CreateAmbientSound(const char* audioDataID)
{
	T_SoundPtr soundPtr(new C_AmbientSoundImpl(ae::base::CalculateHashI32(audioDataID)));
	const S_AudioDataInfo& adi(soundPtr->GetAudioDataInfo());
	if(adi.numChannels == 0)
		DBG_ERR << "Ambient sound has invalid audio data '" << audioDataID << "' !" << std::endl;
	if(adi.numChannels != 2)
		DBG_ERR << "Ambient sound must have stereo audio data '" << audioDataID << "' !" << std::endl;
	else m_ContextImpl->m_Sounds.push_back(soundPtr);
	return new C_Sound(soundPtr, m_ContextImpl);
}

C_Sound* C_Context::CreateSphereAmbientSound(const char* audioDataID)
{
	T_SoundPtr soundPtr(new C_SphereAmbientSoundImpl(ae::base::CalculateHashI32(audioDataID)));
	const S_AudioDataInfo& adi(soundPtr->GetAudioDataInfo());
	if(adi.numChannels == 0)
		DBG_ERR << "Sphere ambient sound has invalid audio data '" << audioDataID << "' !" << std::endl;
	if(adi.numChannels != 2)
		DBG_ERR << "Sphere ambient sound must have stereo audio data '" << audioDataID << "' !" << std::endl;
	else m_ContextImpl->m_Sounds.push_back(soundPtr);
	return new C_Sound(soundPtr, m_ContextImpl);
}


void C_Context::AddListener(C_Listener* pListener)
{
	m_ContextImpl->AddListener(pListener->m_Impl);
}

void C_Context::RemoveListener(C_Listener* pListener)
{
	m_ContextImpl->RemoveListener(pListener->m_Impl);
}

} } // namespace ae { namespace audio {
