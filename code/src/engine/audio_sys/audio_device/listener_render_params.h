#pragma once

#ifndef AE_AUDIO_SYS_LISTENER_RENDER_PARAMS_H
#define AE_AUDIO_SYS_LISTENER_RENDER_PARAMS_H

#include "../../math_sys/types.h"

namespace ae { namespace audio {

	struct S_ListenerRenderParams
	{
		S_ListenerRenderParams() :
			fVolume(1.0f),
			position(0,0,0),
			orientationFront(0,0,1),
			orientationUp(0,1,0),
			velocity(0,0,0)
		{}

		float fVolume;
		ae::math::float3 position;
		ae::math::float3 orientationFront;
		ae::math::float3 orientationUp;
		ae::math::float3 velocity;
	};

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_SYS_LISTENER_RENDER_PARAMS_H
