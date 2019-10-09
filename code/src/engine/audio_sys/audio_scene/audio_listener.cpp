#include "StdAfx.h"

#include "audio_listener.h"

#include "../audio_scene/audio_scene.h"
#include "../audio_sys.h"

#include "../../debug_sys/output/outputs.h"

namespace ae { namespace audio {

C_Listener::C_Listener(T_ListenerPtr& listenerImpl) : m_Impl(listenerImpl)
{
}

C_Listener::~C_Listener()
{
	m_Impl->SetDead();
	C_AudioSys::Instance().Scene().AudioObjectDeleted();
}

} } // namespace ae { namespace audio {
