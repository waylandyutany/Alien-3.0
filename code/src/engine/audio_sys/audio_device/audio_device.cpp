#include "StdAfx.h"

#include "audio_device.h"
#include "../audio_sys_init.h"

#include "../../debug_sys/output/outputs.h"

namespace ae { namespace audio {

C_AudioDevice::C_AudioDevice(const S_Init& sInit) :
	m_Impl(sInit)
{
}

C_AudioDevice::~C_AudioDevice()
{
}

void C_AudioDevice::Render(T_SoundRenderRequests& renderRequests, ae::u32 activeSoundFrame)
{
	m_Impl.Render(renderRequests, activeSoundFrame);
}


} } // namespace ae { namespace audio {
