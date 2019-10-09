#pragma once

#ifndef AE_AUDIO_SYS_SOUND_RENDER_PARAMS_H
#define AE_AUDIO_SYS_SOUND_RENDER_PARAMS_H

#include "../../math_sys/types.h"

namespace ae { namespace audio {

	struct S_ListenerRenderParams;

	struct S_SoundRenderParams
	{
		S_SoundRenderParams() :
			fVolume(1.0f),
			fFrequency(1.0f),
			fLowPassFrequency(22000),
			direction(0,0,0),
			position(0,0,0),
			orientationFront(0,0,1),
			orientationUp(0,1,0),
			velocity(0,0,0),
			listenerParams(0)
		{}

		float fVolume;
		float fFrequency;
		float fLowPassFrequency;
		ae::math::float3 direction;
		ae::math::float3 position;
		ae::math::float3 orientationFront;
		ae::math::float3 orientationUp;
		ae::math::float3 velocity;
		S_ListenerRenderParams* listenerParams;
	};

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_SYS_SOUND_RENDER_PARAMS_H
