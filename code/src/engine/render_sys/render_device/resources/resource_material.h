#pragma once

#ifndef AE_RENDER_SYS_RESOURCE_MATERIAL_H
#define AE_RENDER_SYS_RESOURCE_MATERIAL_H

#ifdef AE_RENDER_DX11
	#include "../dx11/resources/resource_material_dx11.h"
#else
	#error No material resource available !
#endif // #ifdef AE_RENDER_DX11

namespace ae { namespace render {

} } // namespace ae { namespace render {

#endif // #ifdef AE_RENDER_SYS_RESOURCE_MATERIAL_H
