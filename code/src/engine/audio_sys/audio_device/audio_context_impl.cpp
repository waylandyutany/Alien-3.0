#include "StdAfx.h"

#include "audio_context_impl.h"

namespace ae { namespace audio {

void C_ContextImpl::AddListener(T_ListenerPtr& listenerPtr)
{
	AE_ASSERT( std::find(m_Listeners.begin(), m_Listeners.end(), listenerPtr) == m_Listeners.end() && "Context already contains given listener !");
	m_Listeners.push_back(listenerPtr);
}

void C_ContextImpl::RemoveListener(T_ListenerPtr& listenerPtr)
{
	m_Listeners.erase(std::find(m_Listeners.begin(), m_Listeners.end(), listenerPtr));
}

C_ContextImpl::C_ContextImpl( ae::u32 priorityRenderSounds, ae::u32 totalRenderSounds ) : 
	m_bSoundDeleted(false),
	m_bPaused(false),
	m_PriorityRenderSounds(priorityRenderSounds),
	m_TotalRenderSounds(totalRenderSounds)
{
}

void C_ContextImpl::ReleaseDeadObjects()
{
	m_Listeners.erase( std::remove_if(m_Listeners.begin(), m_Listeners.end(), [](const T_ListenerPtr& p) { return p->IsDead(); } ), m_Listeners.end() );

	if(m_bSoundDeleted)
	{
		m_Sounds.erase( std::remove_if(m_Sounds.begin(), m_Sounds.end(), [](const T_SoundPtr& p) { return p->IsDead(); } ), m_Sounds.end() );
		m_bSoundDeleted = false;
	}
}

void C_ContextImpl::Update(float deltaTime)
{
	std::for_each(m_Sounds.begin(), m_Sounds.end(), [deltaTime](T_SoundPtr& p){p->Update(deltaTime);});
}

} } // namespace ae { namespace audio {
