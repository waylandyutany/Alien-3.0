#pragma once

#ifndef AE_AUDIO_SYS_AUDIO_LISTENER_H
#define AE_AUDIO_SYS_AUDIO_LISTENER_H

#include"../audio_device/audio_listener_impl.h"
#include "../../math_sys/types.h"

namespace ae { namespace audio {

	class C_AudioScene;
	class C_Context;

	class C_Listener
	{
		C_Listener();
		NOCOPY_CLASS(C_Listener);

		friend class C_AudioScene;
		friend class C_Context;

		C_Listener(T_ListenerPtr&);
		T_ListenerPtr m_Impl;
	public:
		~C_Listener();

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
		void SetVolume(float volume);
		float GetVolume() const;
// *****************************************************************************************************
	};

} } // namespace ae { namespace audio {

#include "audio_listener.inl"

#endif // #ifdef AE_AUDIO_SYS_AUDIO_LISTENER_H
