#include "StdAfx.h"

#include "scene_sys.h"

namespace ae { namespace scene {

C_SceneSys& C_SceneSys::Inst()
{
	static C_SceneSys instance;
	return instance;
}

} } // namespace ae { namespace scene {
