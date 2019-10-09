#pragma once

#ifndef AE_SCENE_SYS_SCENE_PREFABS_RESOURCE_H
#define AE_SCENE_SYS_SCENE_PREFABS_RESOURCE_H

#include "scene_prefabs.h"

#include "../../resource_sys/resource_sys.h"

namespace ae { namespace scene {

	struct S_ScenePrefabsResource : public ::ae::resource::S_Resource
	{
		S_ScenePrefab* instancies;
	};

} } // namespace ae { namespace scene {

#endif // #ifdef AE_SCENE_SYS_SCENE_PREFABS_RESOURCE_H
