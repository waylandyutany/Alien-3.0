#include "stdafx.h"

#if defined(AE_EDITOR)

#include "combo_box.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
C_ComboBox::C_ComboBox(C_ComboBoxCallback* pCallback, T_WindowStyle cbStyle, C_Widget* pParent, T_ControlID id) :
	C_Widget(pParent, windowType_ComboBox, cbStyle | windowStyle_Visible | windowStyle_TabStop | windowStyle_Child | windowStyle_VScroll, id),
	m_Control(pCallback, *this)
{
}

void C_ComboBox::AddString(const char* string)
{
	m_Control.AddString(string);
}

void C_ComboBox::InsertString(int index, const char* string)
{
	m_Control.InsertString(index, string);
}

void C_ComboBox::Reset()
{
	m_Control.Reset();
}

void C_ComboBox::RemoveString(int index)
{
	m_Control.RemoveString(index);
}

void C_ComboBox::SelectString(int index)
{
	m_Control.SelectString(index);
}

int C_ComboBox::GetStringsCount() const
{
	return m_Control.GetStringsCount();
}

void C_ComboBox::GetString(int index, std::string& str)
{
	m_Control.GetString(index, str);
}

// *****************************************************************************************************
// *****************************************************************************************************
C_DropDownComboBox::C_DropDownComboBox(C_ComboBoxCallback* pCallback, T_WindowStyle cbStyle, C_Widget* pParent, T_ControlID id) :
	C_ComboBox(pCallback, cbStyle | windowStyle_DropDown, pParent, id )
{
}

/*virtual*/ void C_DropDownComboBox::SetLayoutRect(const S_Rect& layoutRect)
{
	S_Rect rect(layoutRect);
	rect.bottom += 128;
	C_ComboBox::SetLayoutRect(rect);
}

void C_DropDownComboBox::ShowDropDown(bool bShow)
{
	m_Control.ShowDropDown(bShow);
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
