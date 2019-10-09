#pragma once

#ifndef AE_INPUT_SYS_WINDOW_CALLBACK_H
#define AE_INPUT_SYS_WINDOW_CALLBACK_H

namespace ae { namespace input {

	class I_WindowCallback
	{
	protected:
		virtual ~I_WindowCallback() {}
	public:
		virtual void OnResize(ae::u32 /*windowWidth*/, ae::u32 /*windowHeight*/) {}
		virtual void OnFocusLost() {};
		virtual void OnFocusSet() {};
		virtual void OnResizeStart() {};
		virtual void OnResizeEnd() {};
		virtual void OnMouseClick() {};
	};

} } // namespace ae { namespace input {

#endif // #ifdef AE_INPUT_SYS_WINDOW_CALLBACK_H
