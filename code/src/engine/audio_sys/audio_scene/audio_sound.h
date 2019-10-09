#pragma once

#ifndef AE_AUDIO_SYS_AUDIO_SOUND_H
#define AE_AUDIO_SYS_AUDIO_SOUND_H

#include"../audio_device/audio_sound_impl.h"
#include"../audio_device/audio_context_impl.h"
#include "../../math_sys/types.h"

namespace ae { namespace audio {

	class C_Context;

	class C_Sound
	{
		C_Sound();
		NOCOPY_CLASS(C_Sound);
		friend class C_Context;

		C_Sound(T_SoundPtr&, T_ContextPtr&);
		T_SoundPtr m_SoundImpl;
		T_ContextPtr m_ContextImpl;
	public:
		~C_Sound();
// *****************************************************************************************************
		void Play(bool looped = false);
		void Stop();
		bool IsPlaying() const;
// *****************************************************************************************************
		void SeekSamplePos(ae::u32 samplePos);
		ae::u32 GetSamplePos() const;
// *****************************************************************************************************
		void SetVolume(float volume);
		float GetVolume() const;
// *****************************************************************************************************
		void SetFrequency(float frequency);
		float GetFrequency() const;
// *****************************************************************************************************
		void SetLowPassFrequency(float frequency);
		float GetLowPassFrequency() const;
// *****************************************************************************************************
		void SetPosition(const ae::math::float3&);
		const ae::math::float3& GetPosition() const;
// *****************************************************************************************************
		void SetVelocity(const ae::math::float3&);
		const ae::math::float3& GetVelocity() const;
// *****************************************************************************************************
		void SetOrientation(const ae::math::float3&, const ae::math::float3&);
		void GetOrientation(ae::math::float3&, ae::math::float3&) const;
// *****************************************************************************************************
	};

} } // namespace ae { namespace audio {

#include "audio_sound.inl"

#endif // #ifdef AE_AUDIO_SYS_AUDIO_SOUND_H
