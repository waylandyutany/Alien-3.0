#pragma once
#if !defined(AE_COMBO_BOX_CTRL_WIN32_H) && defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#define AE_COMBO_BOX_CTRL_WIN32_H

#include "base_ctrl_win32.h"
#include "../../win32/window_win32.h"

namespace ae { namespace windows_sys {
	class C_ComboBoxCallback;
} } // namespace ae { namespace windows_sys {

namespace ae { namespace windows_sys {  namespace win32 {

	class C_ComboBoxCtrl_Win32 : public C_BaseCtrl_Win32
	{
		C_ComboBoxCtrl_Win32();
		NOCOPY_CLASS(C_ComboBoxCtrl_Win32);
		C_ComboBoxCallback* m_pCallback;

		static BOOL CALLBACK EnumChildProc( HWND hwnd, LPARAM lParam );
		static LRESULT CALLBACK ComboBoxEditWndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
		LRESULT CALLBACK LocalComboBoxEditWndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
		WNDPROC m_OldComboBoxEditWndProc;

		void OnComboBoxEditWindow(HWND);
	public:
		C_ComboBoxCtrl_Win32(C_ComboBoxCallback*, C_Window_Win32& control);
		~C_ComboBoxCtrl_Win32();

		void AddString(const char* string);
		void InsertString(int index, const char* string);
		void Reset();
		void RemoveString(int index);
		void SelectString(int index);
		void ShowDropDown(bool bShow);
		int GetStringsCount() const;
		void GetString(int index, std::string& str);

	public:
		virtual S_WPReturn WndProc(UINT umsg, WPARAM wparam, LPARAM lparam, S_WPReturn result);
	};

} } } // namespace ae { namespace windows_sys {  namespace win32 {

namespace ae { namespace windows_sys {
	typedef ae::windows_sys::win32::C_ComboBoxCtrl_Win32 T_ComboBoxCtrl_Impl;
} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_COMBO_BOX_CTRL_WIN32_H) && defined(AE_EDITOR)
