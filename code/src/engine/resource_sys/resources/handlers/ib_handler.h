#pragma once

#ifndef AE_RESOURCE_SYS_IB_LOADER_H
#define AE_RESOURCE_SYS_IB_LOADER_H

#include "../resource_handler.h"

namespace ae { namespace resource {

	class C_IBResourceLoader : public I_ResourceHandler
	{
	public:
		C_IBResourceLoader();
		virtual void Activate();
		virtual void Deactivate();
		virtual void PrintDebug();
	};

} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_IB_LOADER_H
