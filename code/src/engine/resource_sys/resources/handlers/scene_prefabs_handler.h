#pragma once

#ifndef AE_RESOURCE_SYS_SCENE_INSTANCIES_HANDLER_H
#define AE_RESOURCE_SYS_SCENE_INSTANCIES_HANDLER_H

#include "../resource_handler.h"

namespace ae { namespace scene {
	struct S_ScenePrefabBase;
} } // namespace ae { namespace resource {

namespace ae { namespace resource {

	class C_ScenePrefabsResourceHandler : public I_ResourceHandler
	{
		void ActivateNode(ae::scene::S_ScenePrefabBase*);
		void DeactivateNode(ae::scene::S_ScenePrefabBase*);
	public:
		C_ScenePrefabsResourceHandler();
		virtual void Activate();
		virtual void Deactivate();
		virtual void EnumerateElements(I_ResourceElementsEnumerator&);
		virtual void* FindElement(const char* elementName);
		virtual void PrintDebug();
	};

} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_SCENE_INSTANCIES_HANDLER_H
