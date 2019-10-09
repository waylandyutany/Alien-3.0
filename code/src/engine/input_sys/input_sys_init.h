#pragma once

#ifndef AE_INPUT_SYS_INIT_H
#define AE_INPUT_SYS_INIT_H

namespace ae { namespace render {
	class C_RenderWindow;
} } // namespace ae { namespace render {

namespace ae { namespace input {

	struct S_Init
	{
		S_Init(ae::render::C_RenderWindow& wnd) :
			window(wnd)
		{
		}
		ae::render::C_RenderWindow& window;
		private:
			S_Init();
			NOCOPY_CLASS(S_Init);
	};

} } // namespace ae { namespace input {

#endif // #ifdef AE_INPUT_SYS_INIT_H
