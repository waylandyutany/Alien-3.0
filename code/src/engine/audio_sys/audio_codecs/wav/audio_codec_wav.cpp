#include "StdAfx.h"

#ifdef AE_AUDIO_WAV_CODEC

#include "audio_codec_wav.h"

#include "../../../base_sys/streams/i_seekable_input_stream.h"
#include "../../../math_sys/types.h"

namespace ae { namespace audio {

C_AudioCodecWAV::C_AudioCodecWAV(ae::base::I_SeekableInputStream& inputStream) :
	m_SeekableInputStream(inputStream)
{
	size_t readed;
	m_SeekableInputStream.Read(&m_WAVHeader, sizeof(m_WAVHeader), readed);
	m_AudioDataPosition = m_SeekableInputStream.Position();
	m_SamplePosition = 0;

	m_AudioDataInfo.numSamples = m_WAVHeader.dataChunkSize / m_WAVHeader.numChannels / (m_WAVHeader.bitsPerSample >> 3);
	m_AudioDataInfo.numChannels = m_WAVHeader.numChannels;
	m_AudioDataInfo.sampleRate = m_WAVHeader.sampleRate;
	m_AudioDataInfo.numBitesPerSampel = m_WAVHeader.bitsPerSample;
	m_AudioDataInfo.format = E_AF_WAV;
}

void C_AudioCodecWAV::GetCodecInfo(S_AudioDataInfo& audioDataInfo)
{
	audioDataInfo = m_AudioDataInfo;
}

bool C_AudioCodecWAV::ReadSamples(void* samplesBuffer, size_t samplesToRead, size_t& samplesReaded)
{
	samplesToRead = ae::math::Min<>(samplesToRead, m_AudioDataInfo.numSamples - m_SamplePosition);
	AE_ASSERT(samplesToRead <= (m_AudioDataInfo.numSamples - m_SamplePosition) && "Reading too many samples !");

	size_t bytesReaded;
	m_SeekableInputStream.Read(samplesBuffer, m_AudioDataInfo.SamplesToBytes((ae::u32)samplesToRead), bytesReaded);

	samplesReaded = m_AudioDataInfo.BytesToSamples((ae::u32)bytesReaded);
	m_SamplePosition += samplesReaded;

	return true;
}

bool C_AudioCodecWAV::Seek(size_t sampleToSeek)
{
	AE_ASSERT(sampleToSeek < m_AudioDataInfo.numSamples && "Seeking out of valid samples !");
	m_SamplePosition = sampleToSeek;
	return m_SeekableInputStream.Seek(m_AudioDataPosition + m_AudioDataInfo.SamplesToBytes((ae::u32)sampleToSeek));
}

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_WAV_CODEC
