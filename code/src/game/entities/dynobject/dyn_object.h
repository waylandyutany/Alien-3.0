#pragma once

#ifndef AG_ENTITIES_DYN_OBJECT_H
#define AG_ENTITIES_DYN_OBJECT_H

#include "../entity.h"
#include "../../../engine/math_sys/types.h"

namespace ae { namespace scene {
	class C_DynamicSceneNode;
} } // namespace ae { namespace scene {


namespace ag { namespace entities {

// *****************************************************************************************************
// *****************************************************************************************************

	class C_DynObject : public I_Entity
	{
		std::string meshResource;
		ae::math::float3 initialPos;
		ae::scene::C_DynamicSceneNode* pDynamicSceneObject;
		NOCOPY_CLASS(C_DynObject);
	public:
		C_DynObject(const char* meshResource, const ae::math::float3& pos);
		~C_DynObject();

	protected:
		virtual void OnPackageUnloading(const char* packageName);
		virtual void OnPackageLoaded(const char* packageName);
	};

} } // namespace ag { namespace entities {

#endif // #ifdef AG_ENTITIES_DYN_OBJECT_H
