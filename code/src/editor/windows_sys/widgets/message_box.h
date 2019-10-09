#pragma once
#if !defined(AE_MESSAGE_BOX_H) && defined(AE_EDITOR)
#define AE_MESSAGE_BOX_H

#include "../base/window_constants.h"

namespace ae { namespace windows_sys {

	class C_Widget;

	class C_MessageBox
	{
	public:
		static T_MessageBoxReturnCode Show(C_Widget* pParent, T_MessageBoxStyle style, const char* sTitle, const char* sText);
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_MESSAGE_BOX_H) && defined(AE_EDITOR)
