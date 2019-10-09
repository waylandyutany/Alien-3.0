#include "StdAfx.h"

#ifdef AE_AUDIO_OGG_CODEC

#include "audio_codec_ogg.h"

#include "../../../base_sys/streams/i_seekable_input_stream.h"

#include "../../../debug_sys/output/outputs.h"

#ifdef AE_PLATFORM_WIN32
	#ifdef AE_DEBUG
		#pragma comment(lib, "libogg_static_Debug_Win32.lib")
		#pragma comment(lib, "libvorbis_static_Debug_Win32.lib")
		#pragma comment(lib, "libvorbisfile_Debug_Win32.lib")
	#else
		#pragma comment(lib, "libogg_static_Release_Win32.lib")
		#pragma comment(lib, "libvorbis_static_Release_Win32.lib")
		#pragma comment(lib, "libvorbisfile_Release_Win32.lib")
	#endif
#elif AE_PLATFORM_WIN64
	#ifdef AE_DEBUG
		#pragma comment(lib, "libogg_static_Debug_x64.lib")
		#pragma comment(lib, "libvorbis_static_Debug_x64.lib")
		#pragma comment(lib, "libvorbisfile_Debug_x64.lib")
	#else
		#pragma comment(lib, "libogg_static_Release_x64.lib")
		#pragma comment(lib, "libvorbis_static_Release_x64.lib")
		#pragma comment(lib, "libvorbisfile_Release_x64.lib")
	#endif
#else
	#error No platform specified !
#endif


namespace ae { namespace audio {

// *****************************************************************************************************
// *****************************************************************************************************
static size_t read_func( void *ptr, size_t size, size_t nmemb, void *datasource )
{
	C_AudioCodecOGG& codec(*static_cast<C_AudioCodecOGG*>(datasource));
	ae::base::I_SeekableInputStream& inputStream(codec.GetInputStream());
	size_t readed;
	inputStream.Read(ptr, size * nmemb, readed);
	return readed;
}

static int seek_func( void *datasource, ogg_int64_t offset, int whence )
{
	C_AudioCodecOGG& codec(*static_cast<C_AudioCodecOGG*>(datasource));
	ae::base::I_SeekableInputStream& inputStream(codec.GetInputStream());
	switch(whence)
	{
		case SEEK_SET :
			{
				ae::u32 seekOffset((ae::u32)offset);
				return inputStream.Seek(seekOffset) ? 0 : -1;
			}
		case SEEK_CUR :
			{
				ae::u32 seekOffset((ae::u32)(inputStream.Position() + offset));
				return inputStream.Seek(seekOffset) ? 0 : -1;
			}
		case SEEK_END :
			{
				ae::u32 seekOffset((ae::u32)(inputStream.Size() + offset));
				return inputStream.Seek(seekOffset) ? 0 : -1;
			}
	}
	return -1;
}

static int close_func( void* /*datasource*/ )
{
	return 0;
}

static long tell_func( void *datasource )
{
	C_AudioCodecOGG& codec(*static_cast<C_AudioCodecOGG*>(datasource));
	ae::base::I_SeekableInputStream& inputStream(codec.GetInputStream());
	return (long)inputStream.Position();
}

// *****************************************************************************************************
// *****************************************************************************************************

C_AudioCodecOGG::C_AudioCodecOGG(ae::base::I_SeekableInputStream& inputStream) :
	m_SeekableInputStream(inputStream)
{
	ov_callbacks ovCallbacks;
	ovCallbacks.read_func = read_func;
	ovCallbacks.seek_func = seek_func;
	ovCallbacks.close_func = close_func;
	ovCallbacks.tell_func = tell_func;

	if(ov_open_callbacks( this, &m_OggVorbis_File, NULL, 0, ovCallbacks ) != 0)
	{
		DBG_ERR << "ov_open_callbacks failed !" <<std::endl;
	}

	const vorbis_info* info = ov_info( &m_OggVorbis_File, -1 );
	if(!info)
	{
		DBG_ERR << "ov_info failed !" <<std::endl;
	}

	m_AudioDataInfo.numChannels = info->channels;
	m_AudioDataInfo.numBitesPerSampel = 16;
	m_AudioDataInfo.sampleRate = info->rate;
	m_AudioDataInfo.numSamples = (ae::u32)ov_pcm_total( &m_OggVorbis_File, -1 );
	m_AudioDataInfo.format = E_AF_OGG;
}

C_AudioCodecOGG::~C_AudioCodecOGG()
{
	ov_clear(&m_OggVorbis_File);
}

void C_AudioCodecOGG::GetCodecInfo(S_AudioDataInfo& audioDataInfo)
{
	audioDataInfo = m_AudioDataInfo;
}

bool C_AudioCodecOGG::ReadSamples(void* samplesBuffer, size_t samplesToRead, size_t& samplesReaded)
{
	char* dstBuf = reinterpret_cast<char*>(samplesBuffer);
	ae::u32 remainingSamples = (ae::u32)samplesToRead;
	while(remainingSamples > 0)
	{
		int nevim;
		long ret = ov_read( &m_OggVorbis_File, dstBuf, (int)m_AudioDataInfo.SamplesToBytes(remainingSamples), 0, 2, 1, &nevim );
		if ( ret < 0 ) break;
		if ( ret == 0 )	break; // eos
		int bytesCurRead = static_cast<int>(ret);
		int samplesCurRead = (int)m_AudioDataInfo.BytesToSamples( bytesCurRead );
		remainingSamples -= samplesCurRead;
		dstBuf += bytesCurRead;
	}
	samplesReaded = samplesToRead - remainingSamples;
	AE_ASSERT(samplesReaded <= samplesToRead);
	return true;
}

bool C_AudioCodecOGG::Seek(size_t sampleToSeek)
{
	return ov_pcm_seek( &m_OggVorbis_File, sampleToSeek ) == 0 ? true : false;
}

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_OGG_CODEC
