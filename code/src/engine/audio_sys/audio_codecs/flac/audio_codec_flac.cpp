#include "StdAfx.h"

#ifdef AE_AUDIO_FLAC_CODEC

#include "audio_codec_flac.h"

#include "../../../base_sys/streams/i_seekable_input_stream.h"
#include "../../../math_sys/types.h"

#ifdef AE_PLATFORM_WIN32
	#ifdef AE_DEBUG
		#pragma comment(lib, "libFLAC_static_Debug_Win32.lib")
		#pragma comment(lib, "libogg_static_Debug_Win32.lib")
	#else
		#pragma comment(lib, "libFLAC_static_Release_Win32.lib")
		#pragma comment(lib, "libogg_static_Release_Win32.lib")
	#endif
#elif AE_PLATFORM_WIN64
	#ifdef AE_DEBUG
		#pragma comment(lib, "libFLAC_static_Debug_x64.lib")
		#pragma comment(lib, "libogg_static_Debug_x64.lib")
	#else
		#pragma comment(lib, "libFLAC_static_Release_x64.lib")
		#pragma comment(lib, "libogg_static_Release_x64.lib")
	#endif
#else
	#error No platform specified !
#endif

namespace ae { namespace audio {

// *****************************************************************************************************
// *****************************************************************************************************

static FLAC__StreamDecoderReadStatus FLAC_ReadCallback(const FLAC__StreamDecoder* /*decoder*/, FLAC__byte buffer[], size_t *bytes, void *client_data)
{
	C_AudioCodecFLAC& codec(*static_cast<C_AudioCodecFLAC*>(client_data));
	ae::base::I_SeekableInputStream& inputStream(codec.GetInputStream());

	size_t bytesReaded;
	inputStream.Read(buffer, *bytes, bytesReaded);
    *bytes = bytesReaded;

	if(!bytesReaded) return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
    return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
}

static FLAC__StreamDecoderWriteStatus FLAC_WriteCallback(const FLAC__StreamDecoder* /*decoder*/, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data)
{
	C_AudioCodecFLAC& codec(*static_cast<C_AudioCodecFLAC*>(client_data));

    if (frame->header.bits_per_sample == 16)
    {
		const ae::u32 bytesPerSample(frame->header.channels * (frame->header.bits_per_sample >> 3));

        ae::s16 *dst = (ae::s16*)codec.LockWriteBuffer(frame->header.blocksize);
        for (ae::u32 i=0; i < frame->header.blocksize; i++)
        {
            for (ae::u32 j=0; j < frame->header.channels; j++)
            {
                *dst++ = (ae::s16)buffer[j][i];
            }
        }
		codec.UnlockWriteBuffer(frame->header.blocksize * bytesPerSample);
    }

    return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

static FLAC__StreamDecoderSeekStatus FLAC_SeekCallback(const FLAC__StreamDecoder* /*decoder*/, FLAC__uint64 absolute_byte_offset, void *client_data)
{
	C_AudioCodecFLAC& codec(*static_cast<C_AudioCodecFLAC*>(client_data));
	ae::base::I_SeekableInputStream& inputStream(codec.GetInputStream());

	return inputStream.Seek((ae::u32)absolute_byte_offset) ? FLAC__STREAM_DECODER_SEEK_STATUS_OK : FLAC__STREAM_DECODER_SEEK_STATUS_ERROR;
}

static FLAC__StreamDecoderTellStatus FLAC_TellCallback(const FLAC__StreamDecoder* /*decoder*/, FLAC__uint64 *absolute_byte_offset, void *client_data)
{
	C_AudioCodecFLAC& codec(*static_cast<C_AudioCodecFLAC*>(client_data));
	ae::base::I_SeekableInputStream& inputStream(codec.GetInputStream());
	*absolute_byte_offset = (FLAC__uint64)inputStream.Position();
//		return FLAC__STREAM_DECODER_TELL_STATUS_ERROR;
	return FLAC__STREAM_DECODER_TELL_STATUS_OK;
}

static FLAC__StreamDecoderLengthStatus FLAC_LengthCallback(const FLAC__StreamDecoder* /*decoder*/, FLAC__uint64 *stream_length, void *client_data)
{
	C_AudioCodecFLAC& codec(*static_cast<C_AudioCodecFLAC*>(client_data));
	ae::base::I_SeekableInputStream& inputStream(codec.GetInputStream());
	*stream_length  = (FLAC__uint64)inputStream.Size();
//		return FLAC__STREAM_DECODER_LENGTH_STATUS_ERROR;
	return FLAC__STREAM_DECODER_LENGTH_STATUS_OK;
}

static FLAC__bool FLAC_EofCallback(const FLAC__StreamDecoder* /*decoder*/, void *client_data)
{
	C_AudioCodecFLAC& codec(*static_cast<C_AudioCodecFLAC*>(client_data));
	ae::base::I_SeekableInputStream& inputStream(codec.GetInputStream());
	return (inputStream.Position() >= inputStream.Size());
}

static void FLAC_ErrorCallback(const FLAC__StreamDecoder* /*decoder*/, FLAC__StreamDecoderErrorStatus status, void* /* client_data */)
{
//	C_AudioCodecFLAC& codec(*static_cast<C_AudioCodecFLAC*>(client_data));
    switch (status)
    {
		case FLAC__STREAM_DECODER_ERROR_STATUS_LOST_SYNC :
			break;
		case FLAC__STREAM_DECODER_ERROR_STATUS_BAD_HEADER :
			break;
		case FLAC__STREAM_DECODER_ERROR_STATUS_FRAME_CRC_MISMATCH :
			break;
    }
}

static void FLAC_MetadataCallback(const FLAC__StreamDecoder* /*decoder*/, const FLAC__StreamMetadata *metadata, void *client_data)
{
	C_AudioCodecFLAC& codec(*static_cast<C_AudioCodecFLAC*>(client_data));
    if (metadata->type == FLAC__METADATA_TYPE_STREAMINFO)
	{
		S_AudioDataInfo audioDataInfo;

		audioDataInfo.numChannels = metadata->data.stream_info.channels;
		audioDataInfo.numSamples = (ae::u32)metadata->data.stream_info.total_samples;
		audioDataInfo.numBitesPerSampel = metadata->data.stream_info.bits_per_sample;
		audioDataInfo.sampleRate = metadata->data.stream_info.sample_rate;
		audioDataInfo.format = E_AF_FLAC;

		codec.SetAudioDataInfo(audioDataInfo);
	}
    else if (metadata->type == FLAC__METADATA_TYPE_VORBIS_COMMENT)
	{
	}
}

// *****************************************************************************************************
// *****************************************************************************************************

C_AudioCodecFLAC::C_AudioCodecFLAC(ae::base::I_SeekableInputStream& inputStream) :
	m_SeekableInputStream(inputStream),
	m_pStreamDecoder(0),
	m_pFLACBuffer(0)
{
	InitializeFLAC();
}

C_AudioCodecFLAC::C_AudioCodecFLAC(void* pFLACBuffer, ae::base::I_SeekableInputStream& inputStream) : 
	I_AudioCodec(pFLACBuffer),
	m_SeekableInputStream(inputStream),
	m_pStreamDecoder(0),
	m_pFLACBuffer(pFLACBuffer)
{
	InitializeFLAC();
}
 
void C_AudioCodecFLAC::InitializeFLAC()
{
	m_pStreamDecoder = FLAC__stream_decoder_new();
    FLAC__stream_decoder_set_md5_checking(m_pStreamDecoder, false);
    FLAC__stream_decoder_init_stream(	m_pStreamDecoder,
                                        FLAC_ReadCallback,
                                        FLAC_SeekCallback,
                                        FLAC_TellCallback,
                                        FLAC_LengthCallback,
                                        FLAC_EofCallback,
                                        FLAC_WriteCallback,
                                        FLAC_MetadataCallback,
                                        FLAC_ErrorCallback,
                                        this);

    FLAC__stream_decoder_process_until_end_of_metadata(m_pStreamDecoder);
}

C_AudioCodecFLAC::~C_AudioCodecFLAC()
{
    FLAC__stream_decoder_finish(m_pStreamDecoder);
    FLAC__stream_decoder_delete(m_pStreamDecoder);
}

// *****************************************************************************************************
char* C_AudioCodecFLAC::LockWriteBuffer(ae::u32 /*size*/)
{
	m_FLACBytesWritten = 0;
	return static_cast<char*>(m_pFLACBuffer);
}

void C_AudioCodecFLAC::UnlockWriteBuffer(ae::u32 bufferWritten)
{
	m_FLACBytesWritten = bufferWritten;
}

void C_AudioCodecFLAC::SetAudioDataInfo(S_AudioDataInfo& audioDataInfo)
{
	m_AudioDataInfo = audioDataInfo;
}

// *****************************************************************************************************

void C_AudioCodecFLAC::GetCodecInfo(S_AudioDataInfo& audioDataInfo)
{
	audioDataInfo = m_AudioDataInfo;
}

bool C_AudioCodecFLAC::ReadSamples(void* samplesBuffer, size_t samplesToRead, size_t& samplesReaded)
{
	char* dstBuf = reinterpret_cast<char*>(samplesBuffer);
	size_t remainingSamples = samplesToRead;

	FLAC__StreamDecoderState state(FLAC__STREAM_DECODER_UNINITIALIZED);

	while(remainingSamples > 0)
	{
		m_FLACBytesWritten = ae::math::Min<size_t>(m_FLACBytesWritten, m_AudioDataInfo.SamplesToBytes(remainingSamples));
		if(m_FLACBytesWritten)
		{
			memcpy(dstBuf, m_pFLACBuffer, m_FLACBytesWritten);
			size_t samplesCurRead = m_AudioDataInfo.BytesToSamples(m_FLACBytesWritten);
			dstBuf += m_FLACBytesWritten;
			remainingSamples -= samplesCurRead;
			m_FLACBytesWritten = 0;
		}

	    if (state == FLAC__STREAM_DECODER_END_OF_STREAM) break;
		FLAC__stream_decoder_process_single(m_pStreamDecoder);
		state = FLAC__stream_decoder_get_state(m_pStreamDecoder);
	}
	samplesReaded = samplesToRead - remainingSamples;
	AE_ASSERT(samplesReaded <= samplesToRead);
	return true;
}

bool C_AudioCodecFLAC::Seek(size_t sampleToSeek)
{
    return FLAC__stream_decoder_seek_absolute(m_pStreamDecoder, sampleToSeek) ? true : false;
}

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_FLAC_CODEC
