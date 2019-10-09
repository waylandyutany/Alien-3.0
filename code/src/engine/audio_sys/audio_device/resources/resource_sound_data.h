#pragma once

#ifndef AE_AUDIO_SYS_RESOURCE_SOUND_DATA_H
#define AE_AUDIO_SYS_RESOURCE_SOUND_DATA_H

#include "../audio_data_info.h"

#include "../../../resource_sys/resource.h"

namespace ae { namespace audio {

	struct S_SoundDataResource : public ::ae::resource::S_Resource
	{
		S_AudioDataInfo audioDataInfo;
	};

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_SYS_RESOURCE_SOUND_DATA_H
