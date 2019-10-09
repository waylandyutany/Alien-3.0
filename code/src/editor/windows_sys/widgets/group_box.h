#pragma once
#if !defined(AE_GROUP_BOX_H) && defined(AE_EDITOR)
#define AE_GROUP_BOX_H

#include "buttons.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
	class C_GroupBox : public C_Button
	{
		C_GroupBox();
		NOCOPY_CLASS(C_GroupBox);
	protected:
		virtual void GetLayoutRect(S_Rect& layoutRect);
	public:
		C_GroupBox(C_Widget* pParent, const char* sTitle);
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_GROUP_BOX_H) && defined(AE_EDITOR)
