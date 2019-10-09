#pragma once

#ifndef AE_AUDIO_SYS_CONTEXT_RENDER_PARAMS_H
#define AE_AUDIO_SYS_CONTEXT_RENDER_PARAMS_H

#include "../../math_sys/types.h"

namespace ae { namespace audio {

	struct S_ContextRenderParams
	{
		S_ContextRenderParams() :
			fVolume(1.0f)
		{}

		float fVolume;
	};

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_SYS_CONTEXT_RENDER_PARAMS_H
