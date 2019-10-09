#pragma once

#ifndef AE_AUDIO_SYS_AUDIO_SCENE_H
#define AE_AUDIO_SYS_AUDIO_SCENE_H

#include "../audio_device/audio_context_impl.h"
#include "../audio_device/audio_listener_impl.h"

#include "../audio_device/audio_device.h"

#include "../audio_device/audio_sound_render_request.h"

namespace ae { namespace audio {

	class C_Context;
	class C_Listener;
	class C_AudioSys;
	class C_AudioDevice;
	struct S_Init;

	class C_AudioScene
	{
		C_AudioScene();
		~C_AudioScene();
		NOCOPY_CLASS(C_AudioScene);
		friend class C_AudioSys;
// *****************************************************************************************************
		C_AudioScene(const S_Init&, C_AudioDevice&);
// *****************************************************************************************************
		void Update(float deltaTime);
		void ReleaseDeadObjects();
// *****************************************************************************************************
		C_AudioDevice& m_Device;
		T_Contexts m_Contexts;
		T_SoundRenderRequests m_RenderRequests;
		ae::u32 m_ActiveSoundFrame;
		bool m_bAudioObjectDeleted;
		bool m_bPaused;
	public:
		C_Context* CreateContext(ae::u32 priorityRenderSounds, ae::u32 totalRenderSounds);
		C_Listener* CreateListener();
// *****************************************************************************************************
		void AudioObjectDeleted();
// *****************************************************************************************************
		bool IsPaused() const;
		void Pause();
		void Resume();
// *****************************************************************************************************
	};

	inline void C_AudioScene::AudioObjectDeleted() { m_bAudioObjectDeleted = true; }
	inline bool C_AudioScene::IsPaused() const { return m_bPaused;}
	inline void C_AudioScene::Pause() { m_bPaused = true; }
	inline void C_AudioScene::Resume() { m_bPaused = false; }

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_SYS_AUDIO_SCENE_H
