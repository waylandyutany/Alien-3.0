#pragma once
#if !defined(AE_COMBO_BOX_CTRL_H) && defined(AE_EDITOR)
#define AE_COMBO_BOX_CTRL_H

#if (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#include "../win32/controls/combo_box_ctrl_win32.h"
#elif
	#error No window platform implemented !
#endif

#include "../window.h"

namespace ae { namespace windows_sys {

	class C_ComboBoxCallback;

	class C_ComboBoxCtrl
	{
		C_ComboBoxCtrl();
		NOCOPY_CLASS(C_ComboBoxCtrl);
		T_ComboBoxCtrl_Impl m_Impl;
	public:
		C_ComboBoxCtrl(C_ComboBoxCallback*, C_Window& control);
		~C_ComboBoxCtrl();

		void AddString(const char* string);
		void InsertString(int index, const char* string);
		void Reset();
		void RemoveString(int index);
		void SelectString(int index);
		void ShowDropDown(bool bShow);
		int GetStringsCount() const;
		void GetString(int index, std::string& str);
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_COMBO_BOX_CTRL_H) && defined(AE_EDITOR)
