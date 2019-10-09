#pragma once

#ifndef AE_AUDIO_SYS_H
#define AE_AUDIO_SYS_H

#include "audio_sys_init.h"

namespace ae { namespace audio {

	class C_AudioScene;
	class C_AudioDevice;
	class C_AudioResources;

	class C_AudioSys
	{
		static C_AudioSys* pInstance;

		C_AudioSys(const S_Init&);
		~C_AudioSys();

		NOCOPY_CLASS(C_AudioSys);

		C_AudioDevice* m_Device;
		C_AudioScene* m_Scene;
		C_AudioResources* m_Resources;
	public:
// *****************************************************************************************************
		static bool Create(const S_Init&);
		static void Release();
		static C_AudioSys& Instance();
// *****************************************************************************************************
		C_AudioScene& Scene() { return *m_Scene; }
		const C_AudioScene& Scene()  const { return *m_Scene; }
		C_AudioResources& Resources() { return *m_Resources; }
// *****************************************************************************************************
		void Update(float deltaTime);
// *****************************************************************************************************
		bool IsPaused() const;
		void Pause();
		void Resume();
// *****************************************************************************************************
	};

} } // namespace ae { namespace audio {

#endif // #ifdef AE_AUDIO_SYS_H
