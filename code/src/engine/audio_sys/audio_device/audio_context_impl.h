#pragma once

#ifndef AE_AUDIO_SYS_AUDIO_CONTEXT_IMPL_H
#define AE_AUDIO_SYS_AUDIO_CONTEXT_IMPL_H

#include "audio_object_impl.h"
#include "context_render_params.h"

#include "../../base_sys/smart_ptr.h"

#include "audio_sound_impl.h"
#include "audio_listener_impl.h"

namespace ae { namespace audio {

	class C_Context;
	class C_AudioScene;
	class C_ContextImpl : public ae::base::C_RefCounter, public C_AudioObjectImpl
	{
		T_Sounds m_Sounds;
		T_Listeners m_Listeners;

		bool m_bSoundDeleted;
		bool m_bPaused;

		ae::u32 m_PriorityRenderSounds;
		ae::u32 m_TotalRenderSounds;

		S_ContextRenderParams m_RenderParams;

		friend class C_Context;
		friend class C_AudioScene;

		void ReleaseDeadObjects();
		void Update(float deltaTime);
		C_ContextImpl( ae::u32 priorityRenderSounds, ae::u32 totalRenderSounds );
	public:
// *****************************************************************************************************
		const S_ContextRenderParams& GetRenderParams() const;
// *****************************************************************************************************
		void AddListener(T_ListenerPtr&);
		void RemoveListener(T_ListenerPtr&);
// *****************************************************************************************************
		void SoundDeleted();
// *****************************************************************************************************
		void SetRenderSoundsLimits(ae::u32 priorityRenderSounds, ae::u32 totalRenderSounds);
		void GetRenderSoundsLimits(ae::u32& priorityRenderSounds, ae::u32& totalRenderSounds) const;
// *****************************************************************************************************
		void SetVolume(float volume);
		float GetVolume() const;
// *****************************************************************************************************
		bool IsPaused() const;
		void Pause();
		void Resume();
// *****************************************************************************************************
	};

	typedef ae::base::C_SmartPtr<C_ContextImpl> T_ContextPtr;
	typedef std::deque<T_ContextPtr> T_Contexts;

} } // namespace ae { namespace audio {

#include "audio_context_impl.inl"

#endif // #ifdef #ifndef AE_AUDIO_SYS_AUDIO_CONTEXT_IMPL_H

