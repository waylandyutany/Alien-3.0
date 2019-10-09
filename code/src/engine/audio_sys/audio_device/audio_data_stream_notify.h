#pragma once

#ifndef AE_AUDIO_SYS_AUDIO_DATA_STREAM_NOTIFY_H
#define AE_AUDIO_SYS_AUDIO_DATA_STREAM_NOTIFY_H

namespace ae { namespace audio {
	
	class I_AudioDataStreamNotify
	{
	protected:
		virtual ~I_AudioDataStreamNotify() {}
	public:
		virtual void OnStreamDelete() = 0;
	};

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_SYS_AUDIO_DATA_STREAM_NOTIFY_H
