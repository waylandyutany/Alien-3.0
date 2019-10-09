#include "StdAfx.h"

#include "audio_resources.h"

#include "../audio_sys_init.h"

#include "resources/resource_sound_data.h"

#include "audio_data_stream.h"
#include "audio_data_stream_notify.h"

#include "../audio_codecs/wav/audio_codec_wav.h"
#include "../audio_codecs/flac/audio_codec_flac.h"
#include "../audio_codecs/ogg/audio_codec_ogg.h"

#include "../../base_sys/streams/memory_seekable_input_stream.h"
#include "../../base_sys/memory/allocators/incremental_allocator.h"

#include "../../debug_sys/output/outputs.h"
#include "../../_sys/templates.h"

namespace ae { namespace audio {
// *****************************************************************************************************
// *****************************************************************************************************
#ifdef AE_AUDIO_FLAC_CODEC
C_AudioResources::C_AudioResources(const S_Init& sInit) : 
	m_3DMonoFLACBuffers(sInit.FLACBufferSizePerChannel, sInit.maxFLAC3DMonoSoundCodecs),
	m_2DStereoFLACBuffers(sInit.FLACBufferSizePerChannel * 2, sInit.maxFLAC2DStereoSoundCodecs)
#else
C_AudioResources::C_AudioResources(const S_Init&)
#endif // #ifdef AE_AUDIO_FLAC_CODEC
{
#ifdef AE_AUDIO_FLAC_CODEC
	DBG_OUT << "Allocating buffers for FLAC audio codecs. Total bytes " << sInit.FLACBufferSizePerChannel * (sInit.maxFLAC3DMonoSoundCodecs + sInit.maxFLAC2DStereoSoundCodecs * 2) << std::endl;
	DBG_OUT << "    3DMono   buffers : " << sInit.maxFLAC3DMonoSoundCodecs << " Bytes per buffer : " << sInit.FLACBufferSizePerChannel << "  Total bytes : " << sInit.FLACBufferSizePerChannel * sInit.maxFLAC3DMonoSoundCodecs << std::endl;
	DBG_OUT << "    2DStereo buffers : " << sInit.maxFLAC2DStereoSoundCodecs << " Bytes per buffer : " << sInit.FLACBufferSizePerChannel*2 << " Total bytes : " << sInit.FLACBufferSizePerChannel * sInit.maxFLAC2DStereoSoundCodecs*2 << std::endl;

	for(ae::u32 i = 0;i < sInit.maxFLAC3DMonoSoundCodecs; i++)
		m_3DMonoFLACBuffers.AddBuffer(sInit.allocator.Alloc(sInit.FLACBufferSizePerChannel), sInit.FLACBufferSizePerChannel);

	for(ae::u32 i = 0;i < sInit.maxFLAC2DStereoSoundCodecs; i++)
		m_2DStereoFLACBuffers.AddBuffer(sInit.allocator.Alloc(sInit.FLACBufferSizePerChannel * 2), sInit.FLACBufferSizePerChannel * 2);
#endif // #ifdef AE_AUDIO_FLAC_CODEC
}

C_AudioResources::~C_AudioResources()
{
}
// *****************************************************************************************************
// *****************************************************************************************************
S_SoundDataResource* C_AudioResources::FindRecord(ae::base::T_Hash32 soundDataID, S_SDR_Record** precord)
{
	T_SoundDataResourceMap::iterator it(m_SoundDataResources.find(soundDataID));
	if(it == m_SoundDataResources.end()) {
		DBG_ERR << "Unknown sound data resource with given id !" << std::endl;
		return 0;
	}
	if(precord) *precord = &(it->second);
	return (it->second).soundDataResource;
}
// *****************************************************************************************************
bool C_AudioResources::RegisterSoundDataResource(S_SoundDataResource* pSoundDataResource, ae::base::T_Hash32 soundDataID)
{
	if(m_SoundDataResources.find(soundDataID) != m_SoundDataResources.end()) {
		DBG_ERR << "SoundDataResource already registered !" << std::endl;
		return false;
	}

	m_SoundDataResources.insert(T_SoundDataResourcePair(soundDataID, S_SDR_Record(pSoundDataResource)));

	return true;
}

bool C_AudioResources::UnregisterSoundDataResource(S_SoundDataResource* pSoundDataResource, ae::base::T_Hash32 soundDataID)
{
	T_SoundDataResourceMap::iterator it = m_SoundDataResources.find(soundDataID);
	if(it == m_SoundDataResources.end()) {
		DBG_ERR << "Unknown SoundDataResource !" << std::endl;
		return false;
	}

	S_SDR_Record sdrRecord(it->second);
	m_SoundDataResources.erase(it);

// *****************************************************************************************************
	std::for_each(sdrRecord.audioDataStreams.begin(), sdrRecord.audioDataStreams.end(), [&](C_AudioDataStream* pStream)
		{
			DeleteAudioStream(pSoundDataResource, pStream);
		}
	);
// *****************************************************************************************************
	return true;
}

void C_AudioResources::DeleteAudioStream(S_SoundDataResource* pSoundDataResource, C_AudioDataStream* pAudioDataStream)
{
	REFERENCE(pSoundDataResource);
	pAudioDataStream->m_Notificator.OnStreamDelete();

#ifdef AE_AUDIO_FLAC_CODEC
	if(pSoundDataResource->audioDataInfo.format == E_AF_FLAC)
	{
		if(pSoundDataResource->audioDataInfo.numChannels == 1)
			m_3DMonoFLACBuffers.UnacquireBuffer(pAudioDataStream->GetCodec().GetUserData());
		else if(pSoundDataResource->audioDataInfo.numChannels == 2)
			m_2DStereoFLACBuffers.UnacquireBuffer(pAudioDataStream->GetCodec().GetUserData());
		else { AE_ASSERT(!"Unsupported channels count !"); }
	}
#endif // #ifdef AE_AUDIO_FLAC_CODEC

	delete pAudioDataStream->m_AudioCodec;
	delete pAudioDataStream->m_Stream;
	delete pAudioDataStream;
}

// *****************************************************************************************************
// *****************************************************************************************************
C_AudioDataStream* C_AudioResources::AcquireAudioDataStream(ae::base::T_Hash32 soundDataID, I_AudioDataStreamNotify& streamNotificator)
{
	S_SDR_Record* sdrRecord;
	S_SoundDataResource* pSoundDataResource(FindRecord(soundDataID,&sdrRecord));
	if(!pSoundDataResource) return 0;

	char* pAudioData(reinterpret_cast<char*>(pSoundDataResource) + pSoundDataResource->headerSize);
	size_t audioDataSize(pSoundDataResource->totalSize - pSoundDataResource->headerSize);
	I_AudioCodec* pAudioCodec(0);
	ae::base::I_SeekableInputStream* pInputStream = new ae::base::C_MemorySeekableInputStream(pAudioData, audioDataSize);
// *****************************************************************************************************
	if(pSoundDataResource->audioDataInfo.format == E_AF_WAV)
	{
#ifdef AE_AUDIO_WAV_CODEC
		pAudioCodec = new C_AudioCodecWAV(*pInputStream);
#else
		DBG_ERR << "WAV sound type is not supported !" << std::endl;
		return 0;
#endif // #ifdef AE_AUDIO_WAV_CODEC
	}
	else if(pSoundDataResource->audioDataInfo.format == E_AF_FLAC)
	{
#ifdef AE_AUDIO_FLAC_CODEC
		if(pSoundDataResource->audioDataInfo.numChannels == 1)
			pAudioCodec = new C_AudioCodecFLAC(m_3DMonoFLACBuffers.AcquireBuffer(S_Init::FLACBufferSizePerChannel),*pInputStream);
		else if(pSoundDataResource->audioDataInfo.numChannels == 2)
			pAudioCodec = new C_AudioCodecFLAC(m_2DStereoFLACBuffers.AcquireBuffer(S_Init::FLACBufferSizePerChannel * 2),*pInputStream);
		else { AE_ASSERT(!"Unsupported channels count !"); }
#else
		DBG_ERR << "FLAC sound type is not supported !" << std::endl;
		return 0;
#endif // #ifdef AE_AUDIO_OGG_CODEC
	}
	if(pSoundDataResource->audioDataInfo.format == E_AF_OGG)
	{
#ifdef AE_AUDIO_OGG_CODEC
		pAudioCodec = new C_AudioCodecOGG(*pInputStream);
#else
		DBG_ERR << "OGG sound type is not supported !" << std::endl;
		return 0;
#endif // #ifdef AE_AUDIO_OGG_CODEC
	}
// *****************************************************************************************************
	C_AudioDataStream* pAudioDataStream(new C_AudioDataStream(soundDataID, streamNotificator, pInputStream, pAudioCodec));
	sdrRecord->audioDataStreams.push_back(pAudioDataStream);
// *****************************************************************************************************
	return pAudioDataStream;
}

C_AudioDataStream* C_AudioResources::AcquireAudioDataStream(const char* soundDataID, I_AudioDataStreamNotify& streamNotificator)
{
	return AcquireAudioDataStream(ae::base::CalculateHashI32(soundDataID), streamNotificator);
}

void C_AudioResources::UnacquireAudioDataStream(C_AudioDataStream* pAudioDataStream)
{
	S_SDR_Record* sdrRecord;
	S_SoundDataResource* pSoundDataResource(FindRecord(pAudioDataStream->m_SoundDataID, &sdrRecord));
	if(!pSoundDataResource) return;

	ae::RemoveElement(sdrRecord->audioDataStreams, pAudioDataStream);

	DeleteAudioStream(pSoundDataResource, pAudioDataStream);
}

// *****************************************************************************************************
// *****************************************************************************************************
bool C_AudioResources::GetAudioDataInfo(ae::base::T_Hash32 soundDataID, S_AudioDataInfo& audioDataInfo)
{
	S_SoundDataResource* pSoundDataResource(FindRecord(soundDataID,0));
	if(!pSoundDataResource) return false;
	audioDataInfo = pSoundDataResource->audioDataInfo;
	return true;
}

bool C_AudioResources::GetAudioDataInfo(const char* soundDataID, S_AudioDataInfo& audioDataInfo)
{
	return GetAudioDataInfo(ae::base::CalculateHashI32(soundDataID), audioDataInfo);
}

} } // namespace ae { namespace audio {
