#pragma once
#if !defined(AE_EDIT_BOX_H) && defined(AE_EDITOR)
#define AE_EDIT_BOX_H

#include "widget.h"
#include "../base/controls/edit_box_ctrl.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
	class C_EditBoxCallback : public C_BaseWidgetCallback
	{
	public:
		virtual void OnEditChange(T_ControlID) {};
	};
// *****************************************************************************************************
// *****************************************************************************************************
	class C_EditBox : public C_Widget
	{
		C_EditBox();
		NOCOPY_CLASS(C_EditBox);
		C_EditBoxCtrl m_Control;
	public:
		C_EditBox(C_EditBoxCallback*, C_Widget* pParent, T_ControlID);
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_EDIT_BOX_H) && defined(AE_EDITOR)
