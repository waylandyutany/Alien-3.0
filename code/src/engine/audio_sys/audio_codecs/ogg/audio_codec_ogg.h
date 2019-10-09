#pragma once

#ifndef AE_AUDIO_SYS_AUDIO_CODEC_OGG_H
#define AE_AUDIO_SYS_AUDIO_CODEC_OGG_H

#ifdef AE_AUDIO_OGG_CODEC

#include "../i_audio_codec.h"

#include <vorbis/vorbisfile.h>

namespace ae { namespace base {
	class I_SeekableInputStream;
} } // namespace ae { namespace base {

namespace ae { namespace audio {

	class C_AudioCodecOGG : public I_AudioCodec
	{
		NOCOPY_CLASS(C_AudioCodecOGG);

		ae::base::I_SeekableInputStream& m_SeekableInputStream;
		S_AudioDataInfo m_AudioDataInfo;
		OggVorbis_File m_OggVorbis_File;
	public:
		ae::base::I_SeekableInputStream& GetInputStream() { return m_SeekableInputStream; }
		C_AudioCodecOGG(ae::base::I_SeekableInputStream&);
		~C_AudioCodecOGG();
		virtual void GetCodecInfo(S_AudioDataInfo&);
		virtual bool ReadSamples(void* samplesBuffer, size_t samplesToRead, size_t& samplesReaded);
		virtual bool Seek(size_t sampleToSeek);
	};

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_OGG_CODEC

#endif // #ifndef AE_AUDIO_SYS_AUDIO_CODEC_OGG_H
