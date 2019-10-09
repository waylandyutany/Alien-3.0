#pragma once

#ifndef AE_RENDER_SYS_H
#define AE_RENDER_SYS_H

#include "render_sys_init.h"

namespace ae { namespace render {

	class C_RenderWindow;
	class C_RenderScene;
	class C_RenderResources;
	class C_RenderDevice;
	class C_RenderWindowCallback;

	class C_RenderSys
	{
		static C_RenderSys* pInstance;
		C_RenderWindow* m_pWindow;
		C_RenderDevice* m_pDevice;
		C_RenderResources* m_pResources;
		C_RenderScene* m_pScene;
		C_RenderWindowCallback* m_pRenderWindowCallback;
// *****************************************************************************************************
		C_RenderSys(S_Init&);
		~C_RenderSys();
// *****************************************************************************************************
	public:
		C_RenderWindow& GetWindow() { return *m_pWindow; }
		C_RenderScene& GetScene() { return *m_pScene; }
		C_RenderResources& GetResources() { return *m_pResources; }
		bool IsFullScreenMode() const;
// *****************************************************************************************************
		void GetResolution(ae::u32& screenWidth, ae::u32& screenHeight) const;
		void ChangeFullScreenMode(bool fullScreen);
		void ChangeResolution(ae::u32 screenWidth, ae::u32 screenHeight, bool fullScreen);
	public:
// *****************************************************************************************************
		static bool Create(S_Init&);
		static void Release();
		static C_RenderSys& Instance();
// *****************************************************************************************************
		void Update(  );
	};
} } // namespace ae { namespace render {

#endif // #ifdef AE_RENDER_SYS_H
