#pragma once

#ifndef AE_AUDIO_SYS_AUDIO_CODEC_WAV_H
#define AE_AUDIO_SYS_AUDIO_CODEC_WAV_H

#ifdef AE_AUDIO_WAV_CODEC

#include "../i_audio_codec.h"

namespace ae { namespace base {
	class I_SeekableInputStream;
} } // namespace ae { namespace base {

namespace ae { namespace audio {

	class C_AudioCodecWAV : public I_AudioCodec
	{
		C_AudioCodecWAV();
		NOCOPY_CLASS(C_AudioCodecWAV);
		ae::base::I_SeekableInputStream& m_SeekableInputStream;

		#pragma pack(push)
		#pragma pack(1)
		struct S_WAVHeader
		{
			char	chunkID[4];
			ae::u32 chunkSize;
			char	format[4];

			char	fmtChunkID[4];
			ae::u32 fmtChunkSize;
			ae::u16 audioFormat;
			ae::u16 numChannels;
			ae::u32 sampleRate;
			ae::u32 byteRate;
			ae::u16 blockAlign;
			ae::u16 bitsPerSample;

			char	dataChunkID[4];
			ae::u32 dataChunkSize;
		};
		#pragma pack(pop)

		S_WAVHeader m_WAVHeader;
		size_t m_AudioDataPosition;
		size_t m_SamplePosition;
		S_AudioDataInfo m_AudioDataInfo;
	public:
		C_AudioCodecWAV(ae::base::I_SeekableInputStream&);
		virtual void GetCodecInfo(S_AudioDataInfo&);
		virtual bool ReadSamples(void* samplesBuffer, size_t samplesToRead, size_t& samplesReaded);
		virtual bool Seek(size_t sampleToSeek);
	};

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_WAV_CODEC

#endif // #ifndef AE_AUDIO_SYS_AUDIO_CODEC_WAV_H
