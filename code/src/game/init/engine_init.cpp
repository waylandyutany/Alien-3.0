#include "StdAfx.h"

#include "engine_init.h"

#include "../../engine/resource_sys/resource_sys.h"

#include "../../engine/resource_sys/imports/importers/scene_importer.h"
#include "../../engine/resource_sys/imports/importers/prefabs_importer.h"
#include "../../engine/resource_sys/imports/importers/sound_data_importer.h"
#include "../../engine/resource_sys/imports/importers/texture_importer.h"
#include "../../engine/resource_sys/imports/importers/effect_importer.h"
#include "../../engine/resource_sys/imports/importers/collada_importer.h"
#include "../../engine/resource_sys/imports/importers/collada_collision_importer.h"

#include "../../engine/resource_sys/resources/handlers/scene_handler.h"
#include "../../engine/resource_sys/resources/handlers/scene_prefabs_handler.h"
#include "../../engine/resource_sys/resources/handlers/sound_data_handler.h"
#include "../../engine/resource_sys/resources/handlers/texture_handler.h"
#include "../../engine/resource_sys/resources/handlers/effect_handler.h"
#include "../../engine/resource_sys/resources/handlers/vb_handler.h"
#include "../../engine/resource_sys/resources/handlers/ib_handler.h"
#include "../../engine/resource_sys/resources/handlers/mesh_handler.h"
#include "../../engine/resource_sys/resources/handlers/collision_handler.h"

#include "../../engine/debug_sys/output/outputs.h"
#include "../../engine/debug_sys/profiler/profiler.h"

using ae::resource::C_ResourceSys;

namespace ag {

C_AlienEngineInit::C_AlienEngineInit()
{
	InitDebugSys();
	InitFileSys();
	InitResourceSys();
	InitRenderSys();
	InitPhysicSys();
	InitInputSys();
	InitAudioSys();
}

C_AlienEngineInit::~C_AlienEngineInit()
{
	ReleaseAudioSys();
	ReleaseInputSys();
	ReleasePhysicSys();
	ReleaseRenderSys();
	ReleaseResourceSys();
	ReleaseFileSys();
	RelaseDebugSys();
}

void C_AlienEngineInit::InitDebugSys()
{
	m_pFileOutput = new ae::dbg::C_FileOutput("log.txt");
	m_pConsoleOutput = new ae::dbg::C_ConsoleOutput(ae::C_BuildInfo::GetName());

	ae::dbg::doutputs.RegisterOutput(m_pFileOutput);
	ae::dbg::doutputs.RegisterOutput(m_pConsoleOutput);

	DBG_OUT << "Engine name     : " << ae::C_BuildInfo::GetName() << " " << ae::C_BuildInfo::Version() << "." << ae::C_BuildInfo::SubVersion() << std::endl;
	DBG_OUT << "Revision number : " << AE_REVISION_NUMBER << std::endl;
	DBG_OUT << "Revision date   : " << AE_REVISION_DATE << std::endl;
	DBG_OUT << "Repository URL  : " << AE_REPOSITORY_URL << std::endl;
}

void C_AlienEngineInit::InitFileSys()
{
}

void C_AlienEngineInit::InitPhysicSys()
{
	ae::physic::S_Init sInit;
	ae::physic::C_PhysicSys::Create(sInit);
}

void C_AlienEngineInit::InitRenderSys()
{
	const ae::u32 renderSysMemorySize(1024*1024*4); // reserve 4 MB for rendering system
	const ae::u32 contextMemory2D(1024 * 256);
	const ae::u32 contextMemory3D(1024 * 256);

	m_pRenderSysMemory = static_cast<char*>(::malloc(renderSysMemorySize));
	ae::base::C_IncrementalAllocator renderSysAllocator;
	renderSysAllocator.Create(m_pRenderSysMemory, renderSysMemorySize);

	ae::render::S_Init sInit(renderSysAllocator, contextMemory2D, contextMemory3D, 800, 600, 4,32);
	ae::render::C_RenderSys::Create(sInit);
}

void C_AlienEngineInit::InitInputSys()
{
	ae::input::S_Init sInit(ae::render::C_RenderSys::Instance().GetWindow());
	ae::input::C_InputSys::Create(sInit);
}

void C_AlienEngineInit::InitResourceSys()
{
	ae::resource::S_Init sInit("/edit/packages","/resources","/packages");
	ae::resource::C_ResourceSys::Create(sInit);

	m_pSceneImporter = new ae::resource::C_SceneImporter();
	m_pSceneInstanciesImporter = new ae::resource::C_SceneInstanciesImporter();

	m_pSoundDataImporter = new ae::resource::C_SoundDataImporter();
	m_pTextureImporter = new ae::resource::C_TextureImporter();
	m_pEffectImporter = new ae::resource::C_EffectImporter();
	m_pColladaImporter = new ae::resource::C_ColladaImporter();
	m_pColladaCollisionImporter = new ae::resource::C_ColladaCollisionImporter();

	ae::resource::C_Imports& imports(C_ResourceSys::Inst().Imports());
	imports.RegisterImporter(ae::resource::resourceTypeScene,	"scene", "Scene", m_pSceneImporter);
	imports.RegisterImporter(ae::resource::resourceTypePrefabs,	"prefabs", "Scene instancies", m_pSceneInstanciesImporter);
	imports.RegisterImporter(ae::resource::resourceTypeSound,	"wav",  "WAV sound", m_pSoundDataImporter);
	imports.RegisterImporter(ae::resource::resourceTypeSound,	"flac", "FLAC sound", m_pSoundDataImporter);
	imports.RegisterImporter(ae::resource::resourceTypeSound,	"ogg",  "OGG sound", m_pSoundDataImporter);
	imports.RegisterImporter(ae::resource::resourceTypeTexture,	"png",  "Texture", m_pTextureImporter);
	imports.RegisterImporter(ae::resource::resourceTypeTexture,	"jpg",  "Texture", m_pTextureImporter);
	imports.RegisterImporter(ae::resource::resourceTypeTexture,	"tga",  "Texture", m_pTextureImporter);
	imports.RegisterImporter(ae::resource::resourceTypeTexture,	"dds",  "Texture", m_pTextureImporter);
	imports.RegisterImporter(ae::resource::resourceTypeEffect,	"fx",   "Vertex shader", m_pEffectImporter);
	imports.RegisterImporter(ae::resource::resourceTypeMesh,	"dae",  "Model", m_pColladaImporter);
	imports.RegisterImporter(ae::resource::resourceTypeCollision,	"dae",  "Collision model", m_pColladaCollisionImporter);

// *****************************************************************************************************
	m_pSceneResourceHandler = new ae::resource::C_SceneResourceHandler();
	m_pSceneInstanciesResourceHandler = new ae::resource::C_ScenePrefabsResourceHandler();
	m_pSoundDataResourceHandler = new ae::resource::C_SoundDataResourceHandler();
	m_pTextureResourceHandler = new ae::resource::C_TextureResourceLoader();
	m_pEffectResourceHandler = new ae::resource::C_EffectResourceHandler();
	m_pVBResourceHandler = new ae::resource::C_VBResourceHandler();
	m_pIBResourceHandler = new ae::resource::C_IBResourceLoader();
	m_pMeshResourceHandler = new ae::resource::C_MeshResourceHandler();
	m_pCollisionResourceHandler = new ae::resource::C_CollisionResourceHandler();

	ae::resource::C_ResourcesManager& resources(C_ResourceSys::Inst().Resources());
	resources.RegisterHandler(ae::resource::resourceTypeSound, m_pSoundDataResourceHandler);
	resources.RegisterHandler(ae::resource::resourceTypeEffect, m_pEffectResourceHandler);
	resources.RegisterHandler(ae::resource::resourceTypeTexture, m_pTextureResourceHandler);
	resources.RegisterHandler(ae::resource::resourceTypeIB, m_pIBResourceHandler);
	resources.RegisterHandler(ae::resource::resourceTypeVB, m_pVBResourceHandler);
	resources.RegisterHandler(ae::resource::resourceTypeMesh, m_pMeshResourceHandler);
	resources.RegisterHandler(ae::resource::resourceTypeCollision, m_pCollisionResourceHandler);
	resources.RegisterHandler(ae::resource::resourceTypeScene, m_pSceneResourceHandler);
	resources.RegisterHandler(ae::resource::resourceTypePrefabs, m_pSceneInstanciesResourceHandler);
}

void C_AlienEngineInit::InitAudioSys()
{
	const ae::u32 maxReverbs = 3;
	const ae::u32 max3DMonoSounds = 32;
	const ae::u32 max2DStereoSounds = 16;
	const ae::u32 maxTotalSounds = 1024;
	const ae::u32 soundBufferSize = 65536;
	ae::u32 audioSysMemorySize(soundBufferSize * (max3DMonoSounds + max2DStereoSounds) + 1024*1024);
	audioSysMemorySize += ae::audio::S_Init::FLACBufferSizePerChannel * (max3DMonoSounds + max2DStereoSounds * 2);

	m_pAudioSysMemory = static_cast<char*>(::malloc(audioSysMemorySize));
	ae::base::C_IncrementalAllocator audioSysAllocator;
	audioSysAllocator.Create(m_pAudioSysMemory, audioSysMemorySize);
	audioSysAllocator.Reset(true);

	ae::audio::S_Init sInit(maxReverbs, max3DMonoSounds, max2DStereoSounds, max3DMonoSounds, max2DStereoSounds, maxTotalSounds, soundBufferSize, audioSysAllocator);
	ae::audio::C_AudioSys::Create(sInit);
}

void C_AlienEngineInit::ReleaseAudioSys()
{
	ae::audio::C_AudioSys::Release();
	::free(m_pAudioSysMemory);
}

void C_AlienEngineInit::ReleaseResourceSys()
{
	ae::resource::C_ResourcesManager& resources(C_ResourceSys::Inst().Resources());
	resources.UnregisterHandler(ae::resource::resourceTypeScene, m_pSceneResourceHandler);
	resources.UnregisterHandler(ae::resource::resourceTypePrefabs, m_pSceneInstanciesResourceHandler);
	resources.UnregisterHandler(ae::resource::resourceTypeCollision, m_pCollisionResourceHandler);
	resources.UnregisterHandler(ae::resource::resourceTypeIB, m_pIBResourceHandler);
	resources.UnregisterHandler(ae::resource::resourceTypeVB, m_pVBResourceHandler);
	resources.UnregisterHandler(ae::resource::resourceTypeMesh, m_pMeshResourceHandler);
	resources.UnregisterHandler(ae::resource::resourceTypeTexture, m_pTextureResourceHandler);
	resources.UnregisterHandler(ae::resource::resourceTypeEffect, m_pEffectResourceHandler);
	resources.UnregisterHandler(ae::resource::resourceTypeSound, m_pSoundDataResourceHandler);
// *****************************************************************************************************
	ae::resource::C_Imports& imports(C_ResourceSys::Inst().Imports());
	imports.UnregisterImporter(ae::resource::resourceTypeScene);
	imports.UnregisterImporter(ae::resource::resourceTypePrefabs);
	imports.UnregisterImporter(ae::resource::resourceTypeSound);
	imports.UnregisterImporter(ae::resource::resourceTypeTexture);
	imports.UnregisterImporter(ae::resource::resourceTypeEffect);
	imports.UnregisterImporter(ae::resource::resourceTypeMesh);
	imports.UnregisterImporter(ae::resource::resourceTypeCollision);
// *****************************************************************************************************
	delete m_pSoundDataResourceHandler;
	delete m_pTextureResourceHandler;
	delete m_pEffectResourceHandler;
	delete m_pVBResourceHandler;
	delete m_pIBResourceHandler;
	delete m_pMeshResourceHandler;
	delete m_pCollisionResourceHandler;
	delete m_pSceneResourceHandler;
	delete m_pSceneInstanciesResourceHandler;
// *****************************************************************************************************
	delete m_pSceneImporter;
	delete m_pSceneInstanciesImporter;
	delete m_pSoundDataImporter;
	delete m_pTextureImporter;
	delete m_pColladaImporter;
	delete m_pColladaCollisionImporter;
	delete m_pEffectImporter;
// *****************************************************************************************************
	ae::resource::C_ResourceSys::Release();
}

void C_AlienEngineInit::RelaseDebugSys()
{
	ae::dbg::doutputs.UnregisterOutput(m_pConsoleOutput);
	ae::dbg::doutputs.UnregisterOutput(m_pFileOutput);

	delete m_pConsoleOutput;
	delete m_pFileOutput;
}

void C_AlienEngineInit::ReleaseFileSys()
{
}

void C_AlienEngineInit::ReleasePhysicSys()
{
	ae::physic::C_PhysicSys::Release();
}

void C_AlienEngineInit::ReleaseRenderSys()
{
	ae::render::C_RenderSys::Release();
	::free(m_pRenderSysMemory);
}

void C_AlienEngineInit::ReleaseInputSys()
{
	ae::input::C_InputSys::Release();
}

void C_AlienEngineInit::Update(float deltaTime)
{
	PROFILE_BLOCK_BEGIN(Input);
	ae::input::C_InputSys::Inst().Update();
	PROFILE_BLOCK_END(Input);

	PROFILE_BLOCK_BEGIN(Render);
	ae::render::C_RenderSys::Instance().Update();
	PROFILE_BLOCK_END(Render);

	PROFILE_BLOCK_BEGIN(Audio);
	ae::audio::C_AudioSys::Instance().Update(deltaTime);
	PROFILE_BLOCK_END(Audio);

	PROFILE_BLOCK_BEGIN(Physic);
	ae::physic::C_PhysicSys::Instance().Update(deltaTime);
	PROFILE_BLOCK_END(Physic);
}


} // namespace ae {
