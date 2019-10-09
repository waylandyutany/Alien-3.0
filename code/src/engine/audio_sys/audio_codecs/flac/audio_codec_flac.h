#pragma once

#ifndef AE_AUDIO_SYS_AUDIO_CODEC_FLAC_H
#define AE_AUDIO_SYS_AUDIO_CODEC_FLAC_H

#ifdef AE_AUDIO_FLAC_CODEC

#include "../i_audio_codec.h"

#define FLAC__NO_DLL
#include <FLAC/all.h>

namespace ae { namespace base {
	class I_SeekableInputStream;
} } // namespace ae { namespace base {

namespace ae { namespace audio {

	class C_AudioCodecFLAC : public I_AudioCodec
	{
		NOCOPY_CLASS(C_AudioCodecFLAC);
		C_AudioCodecFLAC();
		ae::base::I_SeekableInputStream& m_SeekableInputStream;
        FLAC__StreamDecoder *m_pStreamDecoder;

		void * m_pFLACBuffer;

		size_t m_FLACBytesWritten;
		S_AudioDataInfo m_AudioDataInfo;

		void InitializeFLAC();
	public:
		C_AudioCodecFLAC(ae::base::I_SeekableInputStream&);
		C_AudioCodecFLAC(void* pFLACBuffer, ae::base::I_SeekableInputStream&);

		ae::base::I_SeekableInputStream& GetInputStream() { return m_SeekableInputStream; }

		char* LockWriteBuffer(ae::u32 size);
		void UnlockWriteBuffer(ae::u32 bufferWritten);
		void SetAudioDataInfo(S_AudioDataInfo&);

		~C_AudioCodecFLAC();
		virtual void GetCodecInfo(S_AudioDataInfo&);
		virtual bool ReadSamples(void* samplesBuffer, size_t samplesToRead, size_t& samplesReaded);
		virtual bool Seek(size_t sampleToSeek);
	};

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_FLAC_CODEC

#endif // #ifndef AE_AUDIO_SYS_AUDIO_CODEC_FLAC_H
