#pragma once

#ifndef AE_AUDIO_SYS_AUDIO_CONTEXT_H
#define AE_AUDIO_SYS_AUDIO_CONTEXT_H

#include "../audio_device/audio_context_impl.h"

namespace ae { namespace audio {

	class C_AudioScene;
	class C_Sound;
	class C_Listener;

	class C_Context
	{
		C_Context();
		NOCOPY_CLASS(C_Context);
		friend class C_AudioScene;

		C_Context(T_ContextPtr& contextImpl);
		T_ContextPtr m_ContextImpl;
	public:
		~C_Context();
// *****************************************************************************************************
		C_Sound* Create3DSound(const char* audioDataID);
		C_Sound* CreateAmbientSound(const char* audioDataID);
		C_Sound* CreateSphereAmbientSound(const char* audioDataID);
// *****************************************************************************************************
		void SetVolume(float volume);
		float GetVolume() const;
// *****************************************************************************************************
		void AddListener(C_Listener*);
		void RemoveListener(C_Listener*);
// *****************************************************************************************************
		bool IsPaused() const;
		void Pause();
		void Resume();
// *****************************************************************************************************
	};

} } // namespace ae { namespace audio {

#include "audio_context.inl"

#endif // #ifdef AE_AUDIO_SYS_AUDIO_CONTEXT_H
