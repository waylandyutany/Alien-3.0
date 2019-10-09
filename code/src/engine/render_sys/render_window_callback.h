#pragma once

#ifndef AE_RENDER_SYS_RENDER_WINDOW_CALLBACK_H
#define AE_RENDER_SYS_RENDER_WINDOW_CALLBACK_H

#include "../input_sys/input_window_callback.h"

namespace ae { namespace render {

	class C_RenderScene;
	class C_RenderDevice;
	class C_RenderWindow;

	class C_RenderWindowCallback : public ae::input::I_WindowCallback
	{
		C_RenderWindowCallback();
		NOCOPY_CLASS(C_RenderWindowCallback);

		C_RenderScene& m_RenderScene;
		C_RenderDevice& m_RenderDevice;
		C_RenderWindow& m_RenderWindow;
		ae::u32 m_ResizedWindowWidth, m_ResizedWindowHeight;
		bool m_bResizing;
	public:
		C_RenderWindowCallback(C_RenderScene&, C_RenderDevice&, C_RenderWindow&);
		virtual void OnResize(ae::u32 /*windowWidth*/, ae::u32 /*windowHeight*/);
		virtual void OnFocusLost();
		virtual void OnFocusSet();
		virtual void OnResizeStart();
		virtual void OnResizeEnd();
		virtual void OnMouseClick();
	};

} } // namespace ae { namespace render {

#endif // #ifdef AE_RENDER_SYS_RENDER_WINDOW_CALLBACK_H
