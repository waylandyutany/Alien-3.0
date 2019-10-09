#pragma once
#ifndef AE_PHYSIC_SYS_LIB_PHYSX_H
#define AE_PHYSIC_SYS_LIB_PHYSX_H
// *****************************************************************************************************
// AE_PLATFORM_WIN32
// *****************************************************************************************************
	#ifdef AE_PLATFORM_WIN32
		#ifdef AE_RELEASE
			#pragma comment(lib, "PhysX3_x86.lib")
			#pragma comment(lib, "PhysX3Common_x86.lib")
			#pragma comment(lib, "PhysX3Extensions.lib") 
			#pragma comment(lib, "PxTask.lib")
#ifdef AE_EDITOR
				#pragma comment(lib, "PhysX3Cooking_x86.lib")
#endif // #ifdef AE_EDITOR
		#elif AE_DEVELOP
			#pragma comment(lib, "PhysXVisualDebuggerSDKCHECKED.lib") 
			#pragma comment(lib, "PhysX3ExtensionsCHECKED.lib") 
			#pragma comment(lib, "PhysX3CHECKED_x86.lib") 
			#pragma comment(lib, "PhysX3CommonCHECKED_x86.lib")
			#pragma comment(lib, "PxTaskCHECKED.lib")
#ifdef AE_EDITOR
				#pragma comment(lib, "PhysX3CookingCHECKED_x86.lib")
#endif // #ifdef AE_EDITOR
		#elif AE_DEBUG
			#pragma comment(lib, "PhysXVisualDebuggerSDKDEBUG.lib") 
			#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib") 
			#pragma comment(lib, "PhysX3DEBUG_x86.lib") 
			#pragma comment(lib, "PhysX3CommonDEBUG_x86.lib")
			#pragma comment(lib, "PxTaskDEBUG.lib")
			#ifdef AE_EDITOR
				#pragma comment(lib, "PhysX3CookingDEBUG_x86.lib")
			#endif // #ifdef AE_EDITOR
		#endif // #elif AE_DEBUG
// *****************************************************************************************************
// AE_PLATFORM_WIN64
// *****************************************************************************************************
	#else AE_PLATFORM_WIN64 // #ifdef AE_PLATFORM_WIN32
		#ifdef AE_RELEASE
			#pragma comment(lib, "PhysX3_x64.lib")
			#pragma comment(lib, "PhysX3Common_x64.lib")
			#pragma comment(lib, "PhysX3Extensions.lib") 
			#pragma comment(lib, "PxTask.lib")
#ifdef AE_EDITOR
				#pragma comment(lib, "PhysX3Cooking_x64.lib")
#endif // #ifdef AE_EDITOR
		#elif AE_DEVELOP
			#pragma comment(lib, "PhysXVisualDebuggerSDKCHECKED.lib") 
			#pragma comment(lib, "PhysX3ExtensionsCHECKED.lib") 
			#pragma comment(lib, "PhysX3CHECKED_x64.lib") 
			#pragma comment(lib, "PhysX3CommonCHECKED_x64.lib")
			#pragma comment(lib, "PxTaskCHECKED.lib")
#ifdef AE_EDITOR
				#pragma comment(lib, "PhysX3CookingCHECKED_x64.lib")
#endif // #ifdef AE_EDITOR
		#elif AE_DEBUG
			#pragma comment(lib, "PhysXVisualDebuggerSDKDEBUG.lib") 
			#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib") 
			#pragma comment(lib, "PhysX3DEBUG_x64.lib") 
			#pragma comment(lib, "PhysX3CommonDEBUG_x64.lib")
			#pragma comment(lib, "PxTaskDEBUG.lib")
			#ifdef AE_EDITOR
				#pragma comment(lib, "PhysX3CookingDEBUG_x64.lib")
			#endif // #ifdef AE_EDITOR
		#endif // #elif AE_DEBUG
	#endif // #else AE_PLATFORM_WIN64

#endif // #ifdef AE_PHYSIC_SYS_LIB_PHYSX_H
