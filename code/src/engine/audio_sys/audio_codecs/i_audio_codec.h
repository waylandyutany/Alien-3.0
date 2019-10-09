#pragma once

#ifndef AE_AUDIO_SYS_AUDIO_CODEC_H
#define AE_AUDIO_SYS_AUDIO_CODEC_H

#include "../audio_device/audio_data_info.h"

namespace ae { namespace audio {

	class I_AudioCodec
	{
		NOCOPY_CLASS(I_AudioCodec);
		void* m_UserData;
	protected:
		I_AudioCodec() : m_UserData(0) {};
		I_AudioCodec(void* userData) : m_UserData(userData) {};
	public:
		void* GetUserData() { return m_UserData; }
		virtual ~I_AudioCodec() {};
		virtual void GetCodecInfo(S_AudioDataInfo&) = 0;
		virtual bool ReadSamples(void* samplesBuffer, size_t samplesToRead, size_t& samplesReaded) = 0;
		virtual bool Seek(size_t sampleToSeek) = 0;
	};

} } // namespace ae { namespace audio {

#endif // #ifndef AE_AUDIO_SYS_AUDIO_CODEC_H
