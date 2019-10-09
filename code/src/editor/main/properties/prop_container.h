#pragma once
#if !defined(AE_PROP_CONTAINER_H) && defined(AE_EDITOR)
#define AE_PROP_CONTAINER_H

#include "../../windows_sys/layouts/vbox_layout.h"
#include "../../windows_sys/widgets/group_box.h"
#include "../../windows_sys/widgets/combo_box.h"

namespace ae { namespace editor {

// *****************************************************************************************************
// *****************************************************************************************************
	class I_PropertyPanelCB
	{
	public:
		virtual void OnPropertyChanged() {};
	};

// *****************************************************************************************************
// *****************************************************************************************************
	class C_PropertyPanel : public ae::windows_sys::C_VBoxLayout
	{
	protected:
		C_PropertyPanel();
		virtual ~C_PropertyPanel();
	};

// *****************************************************************************************************
// *****************************************************************************************************
	class C_PropertyContainer : public ae::windows_sys::C_VBoxLayout
	{
		ae::windows_sys::C_GroupBox m_GroupBox;
		ae::windows_sys::C_VBoxLayout m_VLayout;
		ae::windows_sys::C_DropDownComboBox	m_CBProperties;
	public:
		C_PropertyContainer(ae::windows_sys::C_Widget* pParent);
		~C_PropertyContainer();
		void AddPanel(C_PropertyPanel*);
	};

} } //namespace ae { namespace editor {

#endif // #if !defined(AE_PROP_CONTAINER_H) && defined(AE_EDITOR)
