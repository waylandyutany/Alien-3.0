// alien_game.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "engine_init.h"
#include "game_init.h"
#include "editor_init.h"

#include "../../engine/resource_sys/resource_sys.h"

int _tmain(int /*argc*/, _TCHAR* /*argv[]*/)
{
	::ag::C_AlienEngineInit alienEngine;
	ae::base::C_FileSys::Inst().Mount("../../edit","/edit");

#ifdef AE_PLATFORM_WIN32
	ae::base::C_FileSys::Inst().Mount("../../data/tmp_resources/win32","/resources");
#elif AE_PLATFORM_WIN64
	ae::base::C_FileSys::Inst().Mount("../../data/tmp_resources/win64","/resources");
#endif
	ae::base::C_FileSys::Inst().Mount("packages","/packages");

	#if defined(AE_EDITOR)
	::ag::C_AlienEditorInit alienEditor;
	#endif // #if defined(AE_EDITOR)

// *****************************************************************************************************
	C_ResourceSys::Inst().Packages().PackPackageResource("default");
	C_ResourceSys::Inst().Packages().PackPackageResource("tv_buildings");
	C_ResourceSys::Inst().Resources().LoadPackage("default");
	C_ResourceSys::Inst().Resources().LoadPackage("tv_buildings");
// *****************************************************************************************************
	::ag::C_AlienGameInit alienGame;
// *****************************************************************************************************
	float startFrameTime = ae::base::time.GetTimeSeconds();
	Sleep(1);
	float endFrameTime = ae::base::time.GetTimeSeconds();
	ae::u32 frameCounter = 0;
// *****************************************************************************************************
	while(ae::input::C_InputSys::Inst().HandleMsgPump())
	{
		ae::dbg::profiler.BeginFrame();
		const float deltaTime = endFrameTime - startFrameTime;
// *****************************************************************************************************
		alienEngine.Update(deltaTime);
		alienGame.AfterUpdate();
		alienGame.Update(deltaTime);
	#if defined(AE_EDITOR)
		alienEditor.Update(deltaTime);
	#endif // #if defined(AE_EDITOR)
// *****************************************************************************************************
		startFrameTime = endFrameTime;
		endFrameTime = ae::base::time.GetTimeSeconds();
		frameCounter++;
	}
// *****************************************************************************************************
	C_ResourceSys::Inst().Resources().UnloadPackage("tv_buildings");
	C_ResourceSys::Inst().Resources().UnloadPackage("default");
// *****************************************************************************************************
	return 0;
}
