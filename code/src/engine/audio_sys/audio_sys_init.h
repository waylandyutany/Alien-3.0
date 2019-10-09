#pragma once

#ifndef AE_AUDIO_SYS_INIT_H
#define AE_AUDIO_SYS_INIT_H

namespace ae { namespace base {
	class C_IncrementalAllocator;
} } // namespace ae { namespace base {

namespace ae { namespace audio {

	class C_AudioScene;
	class C_AudioDevice;
	class C_AudioResources;

	struct S_Init
	{
		static const ae::u32 FLACBufferSizePerChannel = 8192;

		S_Init(	ae::u32 _maxReverbs,
				ae::u32 _max3DMonoSounds,
				ae::u32 _max2DStereoSounds,
				ae::u32 _maxFLAC3DMonoSoundCodecs,
				ae::u32 _maxFLAC2DStereoSoundCodecs,
				ae::u32 _maxTotalSounds,
				ae::u32 _soundBufferSize,
				ae::base::C_IncrementalAllocator& a) :
			maxReverbs(_maxReverbs),
			max3DMonoSounds(_max3DMonoSounds),
			max2DStereoSounds(_max2DStereoSounds),
			maxFLAC3DMonoSoundCodecs(_maxFLAC3DMonoSoundCodecs),
			maxFLAC2DStereoSoundCodecs(_maxFLAC2DStereoSoundCodecs),
			maxTotalSounds(_maxTotalSounds),
			soundBufferSize(_soundBufferSize),
			allocator(a)
		{}

		ae::u32 maxReverbs;
		ae::u32 max3DMonoSounds;
		ae::u32 max2DStereoSounds;
		ae::u32 maxFLAC3DMonoSoundCodecs;
		ae::u32 maxFLAC2DStereoSoundCodecs;
		ae::u32 maxTotalSounds;
		ae::u32 soundBufferSize;
		ae::base::C_IncrementalAllocator& allocator;
	private:
		NOCOPY_CLASS(S_Init);
		S_Init();
	};

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_SYS_INIT_H
