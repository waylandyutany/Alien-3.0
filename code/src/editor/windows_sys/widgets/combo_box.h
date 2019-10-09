#pragma once
#if !defined(AE_COMBO_BOX_H) && defined(AE_EDITOR)
#define AE_COMBO_BOX_H

#include "widget.h"

#include "../base/controls/combo_box_ctrl.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
	class C_ComboBoxCallback : public C_BaseWidgetCallback
	{
	public:
		virtual void OnStringSelected(T_ControlID, const char* string) {};
		virtual void OnStringRemoved(T_ControlID, const char* string) {};
		virtual void OnStringAdded(T_ControlID, const char* string) {};
		virtual void OnEditChanging(T_ControlID) {}; // When user is changing text inside edit box
		virtual void OnEditChanged(T_ControlID) {}; // When user press enter at the end of text changing
	};

// *****************************************************************************************************
// *****************************************************************************************************
	class C_ComboBox : public C_Widget
	{
		C_ComboBox();
		NOCOPY_CLASS(C_ComboBox);
		C_ComboBoxCallback* m_pCallback;
	protected:
		C_ComboBoxCtrl m_Control;
	public:
		C_ComboBox(C_ComboBoxCallback*, T_WindowStyle, C_Widget* pParent, T_ControlID id);

		void AddString(const char* string);
		void InsertString(int index, const char* string);
		void Reset();
		void RemoveString(int index);
		void SelectString(int index);
		int GetStringsCount() const;
		void GetString(int index, std::string& str);
	};

// *****************************************************************************************************
// *****************************************************************************************************
	class C_DropDownComboBox : public C_ComboBox
	{
		C_DropDownComboBox();
		NOCOPY_CLASS(C_DropDownComboBox);
	protected:
		virtual void SetLayoutRect(const S_Rect& layoutRect);
	public:
		C_DropDownComboBox(C_ComboBoxCallback*, T_WindowStyle, C_Widget* pParent, T_ControlID id);
		void ShowDropDown(bool bShow);
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_COMBO_BOX_H) && defined(AE_EDITOR)
