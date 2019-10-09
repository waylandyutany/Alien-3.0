#include "stdafx.h"

#if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))

#include "combo_box_ctrl_win32.h"
#include "../../../widgets/combo_box.h"

namespace ae { namespace windows_sys {  namespace win32 {

LRESULT CALLBACK C_ComboBoxCtrl_Win32::ComboBoxEditWndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	C_ComboBoxCtrl_Win32* pControl(reinterpret_cast<C_ComboBoxCtrl_Win32*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA)));
	return pControl ? pControl->LocalComboBoxEditWndProc(hwnd, umsg, wparam, lparam) : ::DefWindowProc(hwnd, umsg, wparam, lparam);
}

LRESULT CALLBACK C_ComboBoxCtrl_Win32::LocalComboBoxEditWndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	LRESULT result(m_OldComboBoxEditWndProc(hwnd, umsg, wparam, lparam));
	if(m_pCallback && umsg == WM_CHAR && wparam == 13)
		m_pCallback->OnEditChanged(m_Control.GetControlID());
	return result;
}

BOOL CALLBACK C_ComboBoxCtrl_Win32::EnumChildProc( HWND hwnd, LPARAM lParam )
{
	C_ComboBoxCtrl_Win32* pControl = reinterpret_cast<C_ComboBoxCtrl_Win32*>(lParam);
	pControl->OnComboBoxEditWindow(hwnd);
	return false;
}

void C_ComboBoxCtrl_Win32::OnComboBoxEditWindow(HWND hWnd)
{
	::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
	m_OldComboBoxEditWndProc = (WNDPROC) ::GetWindowLongPtr(hWnd, GWLP_WNDPROC);
	::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)ComboBoxEditWndProc);
}

C_ComboBoxCtrl_Win32::C_ComboBoxCtrl_Win32(C_ComboBoxCallback* pCallback, C_Window_Win32& control) :
	C_BaseCtrl_Win32(control),
	m_pCallback(pCallback),
	m_OldComboBoxEditWndProc(0)
{
	::EnumChildWindows(control, EnumChildProc, (LPARAM)this);
}

C_ComboBoxCtrl_Win32::~C_ComboBoxCtrl_Win32()
{
}

void C_ComboBoxCtrl_Win32::AddString(const char* string)
{
	if(m_pCallback) m_pCallback->OnStringAdded(m_Control.GetControlID(), string);

	::SendMessageA(m_Control, CB_ADDSTRING, 0, (LPARAM)string);
}

void C_ComboBoxCtrl_Win32::InsertString(int index, const char* string)
{
	if(m_pCallback) m_pCallback->OnStringAdded(m_Control.GetControlID(), string);

	::SendMessageA(m_Control, CB_INSERTSTRING, (WPARAM)index, (LPARAM)string);
}

void C_ComboBoxCtrl_Win32::Reset()
{
	::SendMessage(m_Control, CB_RESETCONTENT, 0, 0);
}


void C_ComboBoxCtrl_Win32::RemoveString(int index)
{
	if(m_pCallback)
	{
		std::string str;
		GetString((int)index, str);
		m_pCallback->OnStringRemoved(m_Control.GetControlID(), str.c_str());
	}

	::SendMessage(m_Control, CB_DELETESTRING, (WPARAM)index, 0);
}

void C_ComboBoxCtrl_Win32::SelectString(int index)
{
	::SendMessage(m_Control, CB_SETCURSEL, index, 0);
	if(m_pCallback)
	{
		std::string str;
		GetString((int)index, str);
		m_pCallback->OnStringSelected(m_Control.GetControlID(), str.c_str());
	}
}

void C_ComboBoxCtrl_Win32::ShowDropDown(bool bShow)
{
	::SendMessage(m_Control, CB_SHOWDROPDOWN, bShow, 0);
}

int C_ComboBoxCtrl_Win32::GetStringsCount() const
{
	return (int)::SendMessage(m_Control, CB_GETCOUNT, 0, 0);
}

void C_ComboBoxCtrl_Win32::GetString(int index, std::string& str)
{
	char buffer[::ae::maxStringSize];
	::SendMessageA(m_Control, CB_GETLBTEXT, index, (LPARAM)buffer);
	str = std::string(buffer);
}

/*virtual*/ S_WPReturn C_ComboBoxCtrl_Win32::WndProc(UINT umsg, WPARAM wparam, LPARAM lparam, S_WPReturn result)
{
	if(!m_pCallback) return result;

	if(umsg == WM_COMMAND)
	{
		switch(HIWORD(wparam))
		{
		case CBN_SELCHANGE:
			{
				LRESULT index = ::SendMessage(m_Control, CB_GETCURSEL, 0, 0);
				if(index != CB_ERR)
				{
					std::string str;
					GetString((int)index, str);
					m_pCallback->OnStringSelected(m_Control.GetControlID(), str.c_str());
				}
			}
			break;
		case CBN_EDITCHANGE:
			{
				m_pCallback->OnEditChanging(m_Control.GetControlID());
			}
			break;
		}
	}

	return result;
}

} } } // namespace ae { namespace windows_sys {  namespace win32 {

#endif // #if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
