#include "StdAfx.h"

#include "audio_scene.h"
#include "audio_context.h"
#include "audio_listener.h"
#include "audio_sound.h"

#include "../audio_sys_init.h"
#include "../audio_device/audio_device.h"

#include "../../debug_sys/output/outputs.h"

namespace ae { namespace audio {

C_AudioScene::C_AudioScene(const S_Init& sInit, C_AudioDevice& audioDevice) :
	m_Device(audioDevice),
	m_bAudioObjectDeleted(false),
	m_bPaused(false)
{
	m_RenderRequests.reserve(sInit.max3DMonoSounds + sInit.max2DStereoSounds);
}

C_AudioScene::~C_AudioScene()
{
	m_RenderRequests.clear();
	m_Device.Render(m_RenderRequests, ++m_ActiveSoundFrame);
	ReleaseDeadObjects();
}

void C_AudioScene::Update(float deltaTime)
{
	static ae::u32 activeSoundFrame = 0;
	ReleaseDeadObjects();
	m_ActiveSoundFrame = ++activeSoundFrame;
// *****************************************************************************************************
// Collect render requests
	if(!IsPaused())
	{
		std::for_each(m_Contexts.begin(), m_Contexts.end(),[&](T_ContextPtr& contextPtr)
			{
				C_ContextImpl* contextImpl(contextPtr.get());
				C_ListenerImpl* listenerImpl(contextPtr->m_Listeners[0].get());

				if(!contextImpl->IsPaused())
				{
					std::for_each(contextImpl->m_Sounds.begin(),contextImpl->m_Sounds.end(),[&](T_SoundPtr& soundPtr)
						{
							C_SoundImpl* soundImpl(soundPtr.get());
							if(soundImpl->IsPlaying())
							{
								soundImpl->UpdateSoundRenderParameters(contextImpl->GetRenderParams(), listenerImpl->GetRenderParams());
								soundImpl->SetActiveFrame(m_ActiveSoundFrame);
								m_RenderRequests.push_back(soundImpl);
							}
						}
					);
				}
			}
		);
	}
// *****************************************************************************************************
// Render requests
	m_Device.Render(m_RenderRequests, m_ActiveSoundFrame);
	m_RenderRequests.clear();
// *****************************************************************************************************
// Update sounds after each sound updates it's render params
	if(!IsPaused())
	{
		std::for_each(m_Contexts.begin(), m_Contexts.end(),[deltaTime](T_ContextPtr& contextPtr)
		{ if(!contextPtr->IsPaused()) contextPtr->Update(deltaTime); }
		);
	}
// *****************************************************************************************************
}

void C_AudioScene::ReleaseDeadObjects()
{
	if(!m_bAudioObjectDeleted) return;

	std::for_each(m_Contexts.begin(), m_Contexts.end(), [](const T_ContextPtr& p) { return p->ReleaseDeadObjects();});
	m_Contexts.erase( std::remove_if(m_Contexts.begin(), m_Contexts.end(), [](const T_ContextPtr& p) { return p->IsDead(); } ), m_Contexts.end() );

	m_bAudioObjectDeleted = false;
}

C_Context* C_AudioScene::CreateContext(ae::u32 priorityRenderSounds, ae::u32 totalRenderSounds)
{
	T_ContextPtr contextImpl(new C_ContextImpl(priorityRenderSounds, totalRenderSounds));
	m_Contexts.push_back(contextImpl);
	return new C_Context(contextImpl);
}

C_Listener* C_AudioScene::CreateListener()
{
	T_ListenerPtr listenerImpl(new C_ListenerImpl());
	return new C_Listener(listenerImpl);
}

} } // namespace ae { namespace audio {
