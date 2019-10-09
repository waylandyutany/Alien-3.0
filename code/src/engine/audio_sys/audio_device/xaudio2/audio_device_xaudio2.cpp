#include "StdAfx.h"

#include "audio_device_xaudio2.h"
#include "sound_render_xaudio2.h"

#include "../audio_data_stream.h"

#include "../../audio_sys.h"
#include "../../audio_device/audio_resources.h"
#include "../../audio_codecs/i_audio_codec.h"

#include "../../audio_sys_init.h"

#include "../../../_sys/templates.h"
#include "../../../base_sys/memory/allocators/incremental_allocator.h"

#include "../../../debug_sys/output/outputs.h"
#include "../../../debug_sys/profiler/profiler.h"

namespace ae { namespace audio { namespace xaudio2 {

C_AudioDevice::C_AudioDevice(const S_Init& sInit) :
	m_XAudio2(0),
	m_MasteringVoice(0)
{
	DBG_OUT << "'XAudio2' audio system initializing..." << std::endl;

	HRESULT result;
	result = XAudio2Create( &m_XAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR );
	result = m_XAudio2->GetDeviceDetails(0, &m_DeviceDetails);
    result = m_XAudio2->CreateMasteringVoice( &m_MasteringVoice ); //, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 1 );

    X3DAudioInitialize( m_DeviceDetails.OutputFormat.dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, m_X3DInstance );

	m_3DSoundRenders.reserve(sInit.max3DMonoSounds);
	m_2DSoundRenders.reserve(sInit.max2DStereoSounds);
	m_Reverbs.reserve(sInit.maxReverbs);

	const ae::u32 soundBufferSize(sInit.soundBufferSize >> 1);
// *****************************************************************************************************
	DBG_OUT << "Allocating buffers for audio codecs. Total bytes " <<  sInit.soundBufferSize * (sInit.max3DMonoSounds + sInit.max2DStereoSounds) << std::endl;
	DBG_OUT << "    3DMono   buffers : " << sInit.max3DMonoSounds << " Bytes per buffer : " << sInit.soundBufferSize << " Total bytes : " << sInit.max3DMonoSounds*sInit.soundBufferSize << std::endl;
	DBG_OUT << "    2DStereo buffers : " << sInit.max2DStereoSounds << " Bytes per buffer : " << sInit.soundBufferSize << " Total bytes : " << sInit.max2DStereoSounds*sInit.soundBufferSize << std::endl;
// *****************************************************************************************************

	WAVEFORMATEX wfex;
	wfex.cbSize = sizeof WAVEFORMATEX;
	wfex.wFormatTag = WAVE_FORMAT_PCM;
	wfex.nChannels = 1;
	wfex.nSamplesPerSec = 48000;
	wfex.wBitsPerSample = 16;
	wfex.nBlockAlign = wfex.nChannels * (wfex.wBitsPerSample >> 3);
	wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;

	// Creating 3D mono sound renders
	for(ae::u32 i = 0; i < sInit.max3DMonoSounds; i++)
		m_3DSoundRenders.push_back(new C_SoundRender(m_DeviceDetails, m_XAudio2, sInit.allocator.Alloc(soundBufferSize,wfex.nBlockAlign,false), sInit.allocator.Alloc(soundBufferSize,wfex.nBlockAlign,false),
									soundBufferSize, wfex));

	// Creating 2D stereo sound renders
	wfex.nChannels = 2;
	wfex.nBlockAlign = wfex.nChannels * (wfex.wBitsPerSample >> 3);
	wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;
	for(ae::u32 i = 0; i < sInit.max2DStereoSounds; i++)
		m_2DSoundRenders.push_back(new C_SoundRender(m_DeviceDetails, m_XAudio2, sInit.allocator.Alloc(soundBufferSize,wfex.nBlockAlign,false), sInit.allocator.Alloc(soundBufferSize,wfex.nBlockAlign,false),
									soundBufferSize, wfex));

	// Creating reverb effects
	for(ae::u32 i = 0; i < sInit.maxReverbs; i++)
	{
		IUnknown* pReverb;
		XAudio2CreateReverb(&pReverb);
		m_Reverbs.push_back(pReverb);
	}

	DBG_OUT << "'XAudio2' device info :" << std::endl;
	DBG_OUT << "  Name     : ";
	for(ae::u32 c = 0; c < 256 && m_DeviceDetails.DisplayName[c]; c++)
		DBG_OUT << (char)m_DeviceDetails.DisplayName[c];

	DBG_OUT << std::endl;

	DBG_OUT << "  Channels : " << m_DeviceDetails.OutputFormat.Format.nChannels;
	DBG_OUT << " BPS : " << m_DeviceDetails.OutputFormat.Format.wBitsPerSample;
	DBG_OUT << " Frequency : " << m_DeviceDetails.OutputFormat.Format.nSamplesPerSec << std::endl;

	DBG_OUT << "'XAudio2' audio system initialized." << std::endl;
}

C_AudioDevice::~C_AudioDevice()
{
	DBG_OUT << "'XAudio2' audio system releasing..." << std::endl;

	m_XAudio2->StopEngine();

	for(ae::u32 i = 0; i < m_Reverbs.size(); i++)
		m_Reverbs[i]->Release();

	ae::Purge(m_3DSoundRenders);
	ae::Purge(m_2DSoundRenders);

	m_MasteringVoice->DestroyVoice();
	ae::SafeRelease(m_XAudio2);

	DBG_OUT << "'XAudio2' audio system released." << std::endl;
}

void C_AudioDevice::Render(T_SoundRenderRequests& renderRequests, ae::u32 activeSoundFrame)
{
	// order is important
	ReleaseUnactiveSoundRenders(m_3DSoundRenders, activeSoundFrame);
	ReleaseUnactiveSoundRenders(m_2DSoundRenders, activeSoundFrame);

	// only still active sound renders are updated
	UpdateRenderParams(m_3DSoundRenders);
	UpdateRenderParams(m_2DSoundRenders);

	// create and update new sounds
	CreateNewRenderAssociations(renderRequests);

	XAUDIO2_PERFORMANCE_DATA performanceData;
	m_XAudio2->GetPerformanceData(&performanceData);

	ae::dbg::profiler.SetCounterValue(ae::dbg::profiler.GetCounter("MemoryUsageInBytes"),performanceData.MemoryUsageInBytes);
	ae::dbg::profiler.SetCounterValue(ae::dbg::profiler.GetCounter("CurrentLatencyInSamples"),performanceData.CurrentLatencyInSamples);
	ae::dbg::profiler.SetCounterValue(ae::dbg::profiler.GetCounter("GlitchesSinceEngineStarted"),performanceData.GlitchesSinceEngineStarted);
	ae::dbg::profiler.SetCounterValue(ae::dbg::profiler.GetCounter("ActiveSourceVoiceCount"),performanceData.ActiveSourceVoiceCount);
	ae::dbg::profiler.SetCounterValue(ae::dbg::profiler.GetCounter("TotalSourceVoiceCount"),performanceData.TotalSourceVoiceCount);
	ae::dbg::profiler.SetCounterValue(ae::dbg::profiler.GetCounter("ActiveSubmixVoiceCount"),performanceData.ActiveSubmixVoiceCount);
	ae::dbg::profiler.SetCounterValue(ae::dbg::profiler.GetCounter("ActiveResamplerCount"),performanceData.ActiveResamplerCount);
	ae::dbg::profiler.SetCounterValue(ae::dbg::profiler.GetCounter("ActiveMatrixMixCount"),performanceData.ActiveMatrixMixCount);
}

C_SoundRender* C_AudioDevice::GetFreeSoundRender(bool is3D)
{
	C_SoundRender* pSoundRender(0);
	T_SoundRenders& soundRenders(is3D ? m_3DSoundRenders : m_2DSoundRenders);
	T_SoundRenders::iterator it = soundRenders.begin();
	T_SoundRenders::iterator itEnd = soundRenders.end();

	for(; it != itEnd; ++it)
		if(!(*it)->SoundImpl())
			return pSoundRender = (*it);

	return pSoundRender;
}

void C_AudioDevice::UpdateRenderParams(T_SoundRenders& soundRenders)
{
	S_SoundRenderUpdateParams prm(m_DeviceDetails, m_X3DInstance, m_MasteringVoice);
	std::for_each(soundRenders.begin(), soundRenders.end(), [&prm](C_SoundRender* p) { if(p->IsAcquired()) p->UpdateParams(prm);} );
}

void C_AudioDevice::ReleaseUnactiveSoundRenders(T_SoundRenders& soundRenders, ae::u32 activeSoundFrame)
{
	T_SoundRenders::iterator it(soundRenders.begin());
	T_SoundRenders::iterator itEnd(soundRenders.end());

	for(; it != itEnd; ++it)
	{
		C_SoundRender* pSoundRender(*it);
		C_SoundImpl* pSound(pSoundRender->SoundImpl());

		if(!pSound) continue;
		if(!pSoundRender->IsAcquired()) continue;

		// Active frame mismatch means that sound is no longer active and must be released it's sound render association
		if(pSound->GetActiveFrame() != activeSoundFrame)
			ae::audio::C_AudioSys::Instance().Resources().UnacquireAudioDataStream(pSoundRender->AudioStream());
	}
}

void C_AudioDevice::CreateNewRenderAssociations(T_SoundRenderRequests& renderRequests)
{
	S_SoundRenderUpdateParams prm(m_DeviceDetails, m_X3DInstance, m_MasteringVoice);

	T_SoundRenderRequests::iterator it(renderRequests.begin());
	T_SoundRenderRequests::iterator itEnd(renderRequests.end());
	for(; it != itEnd; ++it)
	{
		C_SoundImpl* pSound(*it);
		if(pSound->GetRenderLink()) continue; // sound already has it's render association

		C_SoundRender* pSoundRender = GetFreeSoundRender(pSound->GetAudioDataInfo().numChannels == 1 ? true : false);
		if(!pSoundRender) continue; // no free sound render available

		C_AudioDataStream* pAudioDataStream = ae::audio::C_AudioSys::Instance().Resources().AcquireAudioDataStream(pSound->GetAudioDataID(), *pSoundRender);
		if(!pAudioDataStream) continue; // no audio data streamavailable

		pSoundRender->Acquire(pSound, pAudioDataStream, prm);
	}
}


} } } // namespace ae { namespace audio { namespace xaudio2 {
