#pragma once
#if !defined(AE_MAIN_WINDOW_WIDGET_H) && defined(AE_EDITOR)
#define AE_MAIN_WINDOW_WIDGET_H

#include "widget.h"

namespace ae { namespace windows_sys {

	class C_MainWindow : public C_Widget
	{
		NOCOPY_CLASS(C_MainWindow);
	protected:
		C_MainWindow();
		~C_MainWindow();
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_MAIN_WINDOW_WIDGET_H) && defined(AE_EDITOR)
