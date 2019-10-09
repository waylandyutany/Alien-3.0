#pragma once

#ifndef AE_AUDIO_SYS_AUDIO_DATA_STREAM_H
#define AE_AUDIO_SYS_AUDIO_DATA_STREAM_H

#include "../../base_sys/hash.h"

namespace ae { namespace base {
	class I_SeekableInputStream;
} } // namespace ae { namespace base {

namespace ae { namespace audio {
	
	class I_AudioCodec;
	class C_AudioResources;

	class C_AudioDataStream
	{
		C_AudioDataStream();
		NOCOPY_CLASS(C_AudioDataStream);

		C_AudioDataStream(ae::base::T_Hash32 soundDataID, I_AudioDataStreamNotify& notificator, ae::base::I_SeekableInputStream* pInputStream, I_AudioCodec* codec) : 
			m_SoundDataID(soundDataID), m_Notificator(notificator), m_Stream(pInputStream), m_AudioCodec(codec) {}

		friend class C_AudioResources;
		ae::base::T_Hash32 m_SoundDataID;
		I_AudioDataStreamNotify& m_Notificator;
		ae::base::I_SeekableInputStream* m_Stream;
		I_AudioCodec* m_AudioCodec;
	public:
		I_AudioCodec& GetCodec() { return *m_AudioCodec; }
	};

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_SYS_AUDIO_DATA_STREAM_H
