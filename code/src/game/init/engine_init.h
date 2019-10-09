#pragma once

#ifndef AG_ENGINE_INIT_H
#define AG_ENGINE_INIT_H

#include "../../engine/_sys/svn_revision.h"
#include "../../engine/_sys/build.h"

#include "../../engine/debug_sys/debug_sys.h"

#include "../../engine/base_sys/file_system/file_system.h"
#include "../../engine/base_sys/time/time.h"

#include "../../engine/render_sys/render_sys.h"
#include "../../engine/render_sys/render_device/render_device.h"
#include "../../engine/render_sys/render_scene/render_scene.h"

#include "../../engine/audio_sys/audio_sys.h"
#include "../../engine/input_sys/input_sys.h"
#include "../../engine/physic_sys/physic_sys.h"

namespace ae { namespace resource { 

	class C_SceneImporter;
	class C_SceneInstanciesImporter;
	class C_EffectImporter;
	class C_SoundDataImporter;
	class C_TextureImporter;
	class C_ColladaImporter;
	class C_ColladaCollisionImporter;

	class C_SceneResourceHandler;
	class C_ScenePrefabsResourceHandler;
	class C_SoundDataResourceHandler;
	class C_TextureResourceLoader;
	class C_EffectResourceHandler;
	class C_VBResourceHandler;
	class C_IBResourceLoader;
	class C_MeshResourceHandler;
	class C_CollisionResourceHandler;

} } // namespace ae { namespace resource { 

namespace ag {

	class C_AlienEngineInit
	{
		void InitDebugSys();
		void InitFileSys();
		void InitRenderSys();
		void InitPhysicSys();
		void InitInputSys();
		void InitResourceSys();
		void InitAudioSys();

		void ReleaseAudioSys();
		void ReleaseResourceSys();
		void RelaseDebugSys();
		void ReleaseFileSys();
		void ReleasePhysicSys();
		void ReleaseRenderSys();
		void ReleaseInputSys();

		ae::dbg::C_FileOutput* m_pFileOutput;
		ae::dbg::C_ConsoleOutput* m_pConsoleOutput;

		char* m_pRenderSysMemory;
		char* m_pAudioSysMemory;

		ae::resource::C_SceneImporter* m_pSceneImporter;
		ae::resource::C_SceneInstanciesImporter* m_pSceneInstanciesImporter;
		ae::resource::C_EffectImporter* m_pEffectImporter;
		ae::resource::C_SoundDataImporter* m_pSoundDataImporter;
		ae::resource::C_TextureImporter* m_pTextureImporter;
		ae::resource::C_ColladaImporter* m_pColladaImporter;
		ae::resource::C_ColladaCollisionImporter* m_pColladaCollisionImporter;

		ae::resource::C_SceneResourceHandler* m_pSceneResourceHandler;
		ae::resource::C_ScenePrefabsResourceHandler* m_pSceneInstanciesResourceHandler;
		ae::resource::C_SoundDataResourceHandler* m_pSoundDataResourceHandler;
		ae::resource::C_TextureResourceLoader* m_pTextureResourceHandler;
		ae::resource::C_EffectResourceHandler* m_pEffectResourceHandler;
		ae::resource::C_VBResourceHandler* m_pVBResourceHandler;
		ae::resource::C_IBResourceLoader* m_pIBResourceHandler;
		ae::resource::C_MeshResourceHandler* m_pMeshResourceHandler;
		ae::resource::C_CollisionResourceHandler* m_pCollisionResourceHandler;

	public:
		C_AlienEngineInit();
		~C_AlienEngineInit();

		void Update(float deltaTime);
	};
} // namespace ae {

#endif // #ifdef AG_ENGINE_INIT_H
