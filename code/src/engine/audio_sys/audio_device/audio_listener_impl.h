#pragma once

#ifndef AE_AUDIO_SYS_AUDIO_LISTENER_IMPL_H
#define AE_AUDIO_SYS_AUDIO_LISTENER_IMPL_H

#include "audio_object_impl.h"
#include "listener_render_params.h"

#include "../../base_sys/smart_ptr.h"
#include "../../math_sys/types.h"

namespace ae { namespace audio {

	class C_ListenerImpl : public ae::base::C_RefCounter, public C_AudioObjectImpl
	{
		S_ListenerRenderParams m_RenderParams;
	public:
		C_ListenerImpl();
		const S_ListenerRenderParams& GetRenderParams() const;
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

	typedef ae::base::C_SmartPtr<C_ListenerImpl> T_ListenerPtr;
	typedef std::deque<T_ListenerPtr> T_Listeners;

} } // namespace ae { namespace audio {

#include "audio_listener_impl.inl"

#endif // #ifdef #ifndef AE_AUDIO_SYS_AUDIO_LISTENER_IMPL_H

