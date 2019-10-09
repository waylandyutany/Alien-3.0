#pragma once
#if !defined(AE_WDIALOG_H) && defined(AE_EDITOR)
#define AE_WDIALOG_H

#include "widget.h"
#include "../base/window_constants.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
	class C_WDialog : public C_Widget
	{
		C_WDialog();
		NOCOPY_CLASS(C_WDialog);
	public:
		typedef ae::u32 T_ReturnCode;
		static const T_ReturnCode returnCancel = 0;
		static const T_ReturnCode returnOK = 1;

		C_WDialog(C_Widget* pParent, const char* sTitle);
		T_ReturnCode DoModal();
	protected:
		T_ReturnCode m_ReturnCode;

		virtual void GetLayoutRect(S_Rect& layoutRect);
		virtual void OnClose();
		void EndDialog(T_ReturnCode);
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_WDIALOG_H) && defined(AE_EDITOR)
