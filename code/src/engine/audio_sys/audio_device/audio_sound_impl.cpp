#include "StdAfx.h"

#include "audio_sound_impl.h"
#include "audio_context_impl.h"
#include "audio_listener_impl.h"

#include "audio_resources.h"
#include "../audio_sys.h"

namespace ae { namespace audio {

// *****************************************************************************************************
// *****************************************************************************************************
C_SoundImpl::C_SoundImpl(ae::base::T_Hash32 audioDataID) :
	m_SamplePos(0),
	m_AudioDataID(audioDataID),
	m_ActiveFrame(0),
	m_pRenderLink(0),
	m_fVolume(1.0f),
	m_fFrequency(1.0f)
{
	m_UpdateSamplePosFunc = &C_SoundImpl::UpdateSamplePosStopped;
	C_AudioSys::Instance().Resources().GetAudioDataInfo(audioDataID, m_AudioDataInfo);
}

void C_SoundImpl::UpdateSoundRenderParameters(const S_ContextRenderParams& context, const S_ListenerRenderParams& listener)
{
	m_RenderParams.fFrequency = m_fFrequency;
	m_RenderParams.fVolume = m_fVolume * context.fVolume * listener.fVolume;
	m_RenderParams.listenerParams = &const_cast<S_ListenerRenderParams&>(listener);
}

void C_SoundImpl::Update(float deltaTime)
{
	(this->*m_UpdateSamplePosFunc)(deltaTime);
}

// *****************************************************************************************************
// *****************************************************************************************************
void C_SoundImpl::UpdateSamplePosStopped(float /*deltaTime*/)
{
}

void C_SoundImpl::UpdateSamplePosLooped(float deltaTime)
{
	m_SamplePos += static_cast<ae::u32>(deltaTime * m_RenderParams.fFrequency * m_AudioDataInfo.sampleRate);
	m_SamplePos %= m_AudioDataInfo.numSamples;
}

void C_SoundImpl::UpdateSamplePosNoLooped(float deltaTime)
{
	m_SamplePos += static_cast<ae::u32>(deltaTime * m_RenderParams.fFrequency * m_AudioDataInfo.sampleRate);
	if(m_SamplePos > m_AudioDataInfo.numSamples)
	{
		m_SamplePos = 0;
		Stop();
	}
}
// *****************************************************************************************************
// *****************************************************************************************************
C_3DSoundImpl::C_3DSoundImpl(ae::base::T_Hash32 audioDataID) :
	C_SoundImpl(audioDataID)
{
}

void C_3DSoundImpl::UpdateSoundRenderParameters(const S_ContextRenderParams& context, const S_ListenerRenderParams& listener)
{
	C_SoundImpl::UpdateSoundRenderParameters(context, listener);
	m_RenderParams.direction = GetPosition() - listener.position;
}

// *****************************************************************************************************
// *****************************************************************************************************
C_AmbientSoundImpl::C_AmbientSoundImpl(ae::base::T_Hash32 audioDataID) :
	C_SoundImpl(audioDataID)
{
}

void C_AmbientSoundImpl::UpdateSoundRenderParameters(const S_ContextRenderParams& context, const S_ListenerRenderParams& listener)
{
	C_SoundImpl::UpdateSoundRenderParameters(context, listener);
}

// *****************************************************************************************************
// *****************************************************************************************************
C_SphereAmbientSoundImpl::C_SphereAmbientSoundImpl(ae::base::T_Hash32 audioDataID) :
	C_SoundImpl(audioDataID)
{
}

void C_SphereAmbientSoundImpl::UpdateSoundRenderParameters(const S_ContextRenderParams& context, const S_ListenerRenderParams& listener)
{
	C_SoundImpl::UpdateSoundRenderParameters(context, listener);
}

// *****************************************************************************************************
// *****************************************************************************************************

} } // namespace ae { namespace audio {
