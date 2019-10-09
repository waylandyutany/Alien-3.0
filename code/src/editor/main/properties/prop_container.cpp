#include "stdafx.h"

#if defined(AE_EDITOR)

#include "prop_container.h"

namespace ae { namespace editor {

C_PropertyPanel::C_PropertyPanel() : 
	ae::windows_sys::C_VBoxLayout(100)
{
}

/*virtual*/ C_PropertyPanel::~C_PropertyPanel()
{
}

C_PropertyContainer::C_PropertyContainer(ae::windows_sys::C_Widget* pParent) :
	ae::windows_sys::C_VBoxLayout(100),
	m_GroupBox(pParent, " Property "),
	m_VLayout(80),
	m_CBProperties(0, ae::windows_sys::windowStyle_LowerCase, pParent, (ae::windows_sys::T_ControlID)200)
{
	AddLayoutItem(&m_GroupBox, 80, 600, 200);
	m_VLayout.AddLayoutItem(&m_CBProperties, 24, 100, 0);
	m_GroupBox.SetLayout(&m_VLayout);
	m_CBProperties.SetText("Transformation");
}

C_PropertyContainer::~C_PropertyContainer()
{
}

void C_PropertyContainer::AddPanel(C_PropertyPanel* pPropertyPanel)
{
	m_VLayout.AddLayoutItem(pPropertyPanel, 100, 200, 0);
}

} } //namespace ae { namespace editor {

#endif // #if defined(AE_EDITOR)
