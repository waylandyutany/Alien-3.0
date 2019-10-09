#pragma once

#ifndef AE_PHYSIC_SYS_H
#define AE_PHYSIC_SYS_H

#include "physic_sys_init.h"
#include "device/device.h"
#include "resources/resources.h"
#include "scene/scene.h"

namespace ae { namespace physic {

	class C_PhysicSys
	{
		static C_PhysicSys* pInstance;
// *****************************************************************************************************
		C_PhysicSys(S_Init&);
		~C_PhysicSys();
// *****************************************************************************************************
		C_Device m_Device;
		C_Resources m_Resources;
		C_Scene m_Scene;
	public:
// *****************************************************************************************************
		static bool Create(S_Init&);
		static void Release();
		static C_PhysicSys& Instance();
// *****************************************************************************************************
		void Update( float deltaTime );
		C_Device& Device() { return m_Device; }
		C_Resources& Resources() { return m_Resources; }
		C_Scene& Scene() { return m_Scene; }
	};
} } // namespace ae { namespace physic {

#endif // #ifdef AE_PHYSIC_SYS_H
