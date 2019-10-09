#pragma once

#ifndef AE_AUDIO_SYS_AUDIO_DATA_H
#define AE_AUDIO_SYS_AUDIO_DATA_H

namespace ae { namespace audio {

	enum E_AudioFormat
	{
		E_AF_OGG,
		E_AF_FLAC,
		E_AF_WAV,
		E_AF_INVALID = 0xffffffff // force 4 byte enum size
	};

	struct S_AudioDataInfo
	{
		E_AudioFormat format;

		size_t numChannels;
		size_t numSamples;
		size_t numBitesPerSampel;
		size_t sampleRate;

		size_t SamplesToBytes(size_t samples) const;
		size_t BytesToSamples(size_t bytes) const;
		S_AudioDataInfo() : format(E_AF_INVALID), numChannels(0), numSamples(0), numBitesPerSampel(0), sampleRate(0) {}
	};

	inline size_t S_AudioDataInfo::SamplesToBytes(size_t samples) const
	{
		return samples * numChannels * (numBitesPerSampel >> 3);
	}

	inline size_t S_AudioDataInfo::BytesToSamples(size_t bytes) const
	{
		return bytes / (numChannels * (numBitesPerSampel >> 3));
	}

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_SYS_AUDIO_DATA_H
