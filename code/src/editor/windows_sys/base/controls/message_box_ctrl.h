#pragma once
#if !defined(AE_MESSAGE_BOX_CTRL_H) && defined(AE_EDITOR)
#define AE_MESSAGE_BOX_CTRL_H

#include "../../base/window_constants.h"

namespace ae { namespace windows_sys {

	class C_Window;

	class C_MessageBoxCtrl
	{
	public:
		static T_MessageBoxReturnCode Show(C_Window* pParent, T_MessageBoxStyle style, const char* sTitle, const char* sText);
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_MESSAGE_BOX_CTRL_H) && defined(AE_EDITOR)
