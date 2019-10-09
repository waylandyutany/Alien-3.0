#include "StdAfx.h"

#include "audio_sys.h"

#include "audio_scene/audio_scene.h"
#include "audio_device/audio_device.h"
#include "audio_device/audio_resources.h"

namespace ae { namespace audio {

C_AudioSys* C_AudioSys::pInstance = 0;

C_AudioSys::C_AudioSys(const S_Init& sInit) :
	m_Device(new C_AudioDevice(sInit)),
	m_Scene(new C_AudioScene(sInit, *m_Device)),
	m_Resources(new C_AudioResources(sInit))
{
}

C_AudioSys::~C_AudioSys()
{
	delete m_Resources;
	delete m_Scene;
	delete m_Device;
}

bool C_AudioSys::Create(const S_Init& sInit)
{
	AE_ASSERT(!pInstance && "C_AudioSys instance already created !");
	pInstance = new C_AudioSys(sInit);
	return true;
}

void C_AudioSys::Release()
{
	AE_ASSERT(pInstance && "Uninitialized C_AudioSys class instance !");
	delete pInstance;
}

C_AudioSys& C_AudioSys::Instance()
{
	AE_ASSERT(pInstance && "Uninitialized C_AudioSys class instance !");
	return *pInstance;
}

void C_AudioSys::Update(float deltaTime)
{
	m_Scene->Update(deltaTime);
}

// *****************************************************************************************************
bool C_AudioSys::IsPaused() const
{
	return Scene().IsPaused();
}

void C_AudioSys::Pause()
{
	return Scene().Pause();
}

void C_AudioSys::Resume()
{
	return Scene().Resume();
}
// *****************************************************************************************************

} } // namespace ae { namespace audio {
