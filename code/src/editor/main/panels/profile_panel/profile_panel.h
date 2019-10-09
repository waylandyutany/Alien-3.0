#pragma once
#if !defined(AE_PROFILE_PANEL_H) && defined(AE_EDITOR)
#define AE_PROFILE_PANEL_H

#include "../base_panel.h"

namespace ae { namespace editor {
// *****************************************************************************************************
// *****************************************************************************************************
	class C_ProfilePanel : public C_BasePanel
	{
		C_ProfilePanel();
		NOCOPY_CLASS(C_ProfilePanel);
		ae::windows_sys::C_StaticText m_ProfileText;
	public:
// *****************************************************************************************************
		C_ProfilePanel(C_Widget*);
		~C_ProfilePanel();
// *****************************************************************************************************
		virtual void Activate();
		virtual void Deactivate();
		virtual void OnTimer(ae::u32 timerID);
// *****************************************************************************************************
	};

} } //namespace ae { namespace editor {

#endif // #if !defined(AE_PROFILE_PANEL_H) && defined(AE_EDITOR)
