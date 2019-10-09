#pragma once

#ifndef AE_AUDIO_SYS_RESOURCES_H
#define AE_AUDIO_SYS_RESOURCES_H

#include "../../base_sys/hash.h"
#include "../../base_sys/memory/reusable_buffers.h"

namespace ae { namespace audio {

	class C_AudioSys;
	class C_AudioDataStream;
	class I_AudioDataStreamNotify;

	struct S_AudioDataInfo;
	struct S_Init;
	struct S_SoundDataResource;

	class C_AudioResources
	{
		C_AudioResources();
		NOCOPY_CLASS(C_AudioResources);
// *****************************************************************************************************
		C_AudioResources(const S_Init&);
		~C_AudioResources();
// *****************************************************************************************************
		typedef std::list<C_AudioDataStream*> T_AudioDataStreams;
		struct S_SDR_Record
		{
			S_SDR_Record(S_SoundDataResource* _soundDataResource) :
				soundDataResource(_soundDataResource) {}

			S_SoundDataResource* soundDataResource;
			T_AudioDataStreams audioDataStreams;
		private:
			S_SDR_Record();
		};
// *****************************************************************************************************
		typedef std::pair<ae::base::T_Hash32, S_SDR_Record> T_SoundDataResourcePair;
		typedef std::map<ae::base::T_Hash32, S_SDR_Record> T_SoundDataResourceMap;
		T_SoundDataResourceMap m_SoundDataResources;
// *****************************************************************************************************
#ifdef AE_AUDIO_FLAC_CODEC
		ae::base::C_ReusableBuffers m_3DMonoFLACBuffers;
		ae::base::C_ReusableBuffers m_2DStereoFLACBuffers;
#endif // #ifdef AE_AUDIO_FLAC_CODEC
// *****************************************************************************************************
		S_SoundDataResource* FindRecord(ae::base::T_Hash32 soundDataID, S_SDR_Record**);
		void DeleteAudioStream(S_SoundDataResource*, C_AudioDataStream*);
// *****************************************************************************************************
		friend class C_AudioSys;
	public:
// *****************************************************************************************************
		bool RegisterSoundDataResource(S_SoundDataResource*, ae::base::T_Hash32 soundDataID);
		bool UnregisterSoundDataResource(S_SoundDataResource*, ae::base::T_Hash32 soundDataID);
// *****************************************************************************************************
		C_AudioDataStream* AcquireAudioDataStream(ae::base::T_Hash32 soundDataID, I_AudioDataStreamNotify&);
		C_AudioDataStream* AcquireAudioDataStream(const char*, I_AudioDataStreamNotify&);
		void UnacquireAudioDataStream(C_AudioDataStream*);
// *****************************************************************************************************
		bool GetAudioDataInfo(ae::base::T_Hash32 soundDataID, S_AudioDataInfo&);
		bool GetAudioDataInfo(const char* soundDataID, S_AudioDataInfo&);
// *****************************************************************************************************
	};

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_SYS_H
