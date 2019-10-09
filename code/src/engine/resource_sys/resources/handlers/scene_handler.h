#pragma once

#ifndef AE_RESOURCE_SYS_SCENE_HANDLER_H
#define AE_RESOURCE_SYS_SCENE_HANDLER_H

#include "../resource_handler.h"

namespace ae { namespace resource {

	class C_SceneResourceHandler : public I_ResourceHandler
	{
	public:
		C_SceneResourceHandler();
		virtual void Activate();
		virtual void Deactivate();
		virtual void EnumerateElements(I_ResourceElementsEnumerator&);
		virtual void PrintDebug();
	};

} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_SCENE_HANDLER_H
