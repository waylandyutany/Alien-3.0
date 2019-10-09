#pragma once

#ifndef AE_RENDER_SYS_INIT_H
#define AE_RENDER_SYS_INIT_H

namespace ae { namespace base {
	class C_IncrementalAllocator;
} } // namespace ae { namespace base {

namespace ae { namespace render {

	struct S_Init
	{
		S_Init( ae::base::C_IncrementalAllocator& a, ae::u32 ctxMem2D, ae::u32 ctxMem3D, ae::u32 wWidth, ae::u32 wHeight, ae::u32 iMaxDirLights, ae::u32 iMaxPointLights ) :
			allocator(a),
			contextMemory2D(ctxMem2D),
			contextMemory3D(ctxMem3D),
			windowWidth(wWidth),
			windowHeight(wHeight),
			maxDirLights(iMaxDirLights), maxPointLights(iMaxPointLights)
		{
		}

		ae::base::C_IncrementalAllocator& allocator;
		ae::u32 contextMemory2D;
		ae::u32 contextMemory3D;
		ae::u32 windowWidth, windowHeight;
		ae::u32 maxDirLights, maxPointLights;
		private:
			S_Init();
			NOCOPY_CLASS(S_Init);
	};

} } // namespace ae { namespace render {

#endif // #ifdef AE_RENDER_SYS_INIT_H
