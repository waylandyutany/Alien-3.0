#pragma once

#ifndef AE_RENDER_SYS_RENDER_DEVICE_H
#define AE_RENDER_SYS_RENDER_DEVICE_H

#ifdef AE_RENDER_DX11
	#include "dx11/render_device_dx11.h"
#else
	#error No render device available !
#endif // #ifdef AE_RENDER_DX11

namespace ae { namespace render {

	class C_RenderSys;
	class C_RenderWindow;
	class C_RenderWindowCallback;
	struct S_Init;

	class C_RenderDevice
	{
		friend class ae::render::C_RenderSys;
		friend class C_RenderScene;
		dx11::C_RenderDevice* m_Impl;
		C_RenderWindow* m_Window;
// *****************************************************************************************************
		C_RenderDevice(const S_Init&, C_RenderWindow*);
		~C_RenderDevice();
	public:
// *****************************************************************************************************
	#ifdef AE_RENDER_DX11
		dx11::C_RenderDevice* GetImpl() { return m_Impl; }
	#endif // #ifdef AE_RENDER_DX11
// *****************************************************************************************************
		void BeginRender();
		void BeginSceneRender();
		void BeginStencilShadowsRender();
		void BeginShadowMapRender();
		void BeginLightRender();
		void BeginDfrComposeRender();
		void Begin2DRender();
		void EndRender();
// *****************************************************************************************************
		bool IsFullScreenMode() const;
		void ChangeFullScreenMode(bool fullScreen);
		void ChangeResolution(ae::u32 screenWidth, ae::u32 screenHeight, bool fullScreen);
	private:
		void ResizeBuffers(ae::u32 viewWidth, ae::u32 viewHeight);
		const C_RenderDevice& operator = (const C_RenderDevice&);
		C_RenderDevice(const C_RenderDevice&);
	};

} } // namespace ae { namespace render {

#endif // #ifdef AE_RENDER_SYS_RENDER_DEVICE_H
