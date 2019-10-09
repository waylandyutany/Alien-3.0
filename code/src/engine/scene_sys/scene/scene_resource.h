#pragma once

#ifndef AE_SCENE_SYS_SCENE_RESOURCE_H
#define AE_SCENE_SYS_SCENE_RESOURCE_H

#include "../../resource_sys/resource_sys.h"
#include "scene.h"

namespace ae { namespace scene {

	struct S_SceneResource : public ::ae::resource::S_Resource
	{
		S_SceneNode* pNode;
	};

} } // namespace ae { namespace scene {

#endif // #ifdef AE_SCENE_SYS_SCENE_RESOURCE_H
