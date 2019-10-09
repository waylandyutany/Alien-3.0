#pragma once

#ifndef AE_SCENE_SYS_H
#define AE_SCENE_SYS_H

#include "scene/scene.h"

namespace ae { namespace scene {

	class C_SceneSys
	{
		C_SceneSys() {}
		NOCOPY_CLASS(C_SceneSys);
		C_Scene m_Scene;
	public:
		static C_SceneSys& Inst();
		C_Scene& Scene() { return m_Scene; }
	};

} } // namespace ae { namespace scene {

#endif // #ifdef AE_SCENE_SYS_H
