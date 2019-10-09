#include "stdafx.h"

#if defined(AE_EDITOR)

#include "combo_box_ctrl.h"

namespace ae { namespace windows_sys {

C_ComboBoxCtrl::C_ComboBoxCtrl(C_ComboBoxCallback* pCallBack, C_Window& control) : m_Impl(pCallBack, control)
	
{
}

C_ComboBoxCtrl::~C_ComboBoxCtrl()
{
}

void C_ComboBoxCtrl::AddString(const char* string)
{
	m_Impl.AddString(string);
}

void C_ComboBoxCtrl::InsertString(int index, const char* string)
{
	m_Impl.InsertString(index, string);
}

void C_ComboBoxCtrl::Reset()
{
	m_Impl.Reset();
}

void C_ComboBoxCtrl::RemoveString(int index)
{
	m_Impl.RemoveString(index);
}

void C_ComboBoxCtrl::SelectString(int index)
{
	m_Impl.SelectString(index);
}

void C_ComboBoxCtrl::ShowDropDown(bool bShow)
{
	m_Impl.ShowDropDown(bShow);
}

int C_ComboBoxCtrl::GetStringsCount() const
{
	return m_Impl.GetStringsCount();
}

void C_ComboBoxCtrl::GetString(int index, std::string& str)
{
	m_Impl.GetString(index, str);
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
