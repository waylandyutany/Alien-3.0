#include "StdAfx.h"

#include "dyn_object.h"
#include "../../../engine/scene_sys/scene_sys.h"

namespace ag { namespace entities {

C_DynObject::C_DynObject(const char* meshResource, const ae::math::float3& pos) : 
		meshResource(meshResource), 
		initialPos(pos),
		pDynamicSceneObject(0)
{
	pDynamicSceneObject = ae::scene::C_SceneSys::Inst().Scene().CreateDynamicNode(meshResource, pos);
}

C_DynObject::~C_DynObject()
{
	if(pDynamicSceneObject)
		ae::scene::C_SceneSys::Inst().Scene().DestroyDynamicNode(pDynamicSceneObject);
}

/*virtual*/ void C_DynObject::OnPackageUnloading(const char* packageName)
{
	if(pDynamicSceneObject)
	{
		ae::scene::C_SceneSys::Inst().Scene().DestroyDynamicNode(pDynamicSceneObject);
		pDynamicSceneObject = 0;
	}
}

/*virtual*/ void C_DynObject::OnPackageLoaded(const char* packageName)
{
	if(!pDynamicSceneObject)
		pDynamicSceneObject = ae::scene::C_SceneSys::Inst().Scene().CreateDynamicNode(meshResource.c_str(), initialPos);
}

} } // namespace ag { namespace entities {

