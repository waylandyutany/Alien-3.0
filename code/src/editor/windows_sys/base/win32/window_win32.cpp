#include "stdafx.h"

#if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))

#include "window_win32.h"
#include "controls/base_ctrl_win32.h"

namespace ae { namespace windows_sys {  namespace win32 {

namespace windowclass {
	
	class C_WindowClass
	{
		WNDCLASSEX windowClass;
	public:
		C_WindowClass();
		~C_WindowClass();

		static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
	};

	C_WindowClass::C_WindowClass()
	{
		INITCOMMONCONTROLSEX initCommonControlsEx;
		initCommonControlsEx.dwSize = sizeof(initCommonControlsEx);
		initCommonControlsEx.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES | ICC_STANDARD_CLASSES;

		::InitCommonControlsEx(&initCommonControlsEx);

		windowClass.style			= CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc		= WndProc;
		windowClass.cbClsExtra		= 0;
		windowClass.cbWndExtra		= 0;
		windowClass.hInstance		= GetModuleHandle(NULL);
		windowClass.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
		windowClass.hIconSm			= windowClass.hIcon;
		windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground	= (HBRUSH)COLOR_WINDOW;//(HBRUSH)GetStockObject(BLACK_BRUSH);
		windowClass.lpszMenuName	= NULL;
		windowClass.lpszClassName	= ae::editor::C_BuildInfo::GetNameW();
		windowClass.cbSize			= sizeof(WNDCLASSEX);
	
		if(!RegisterClassEx(&windowClass))
		{
			AE_ASSERT(0 && "Unable to register 'Alien editor' window class !");
		}
	}

	C_WindowClass::~C_WindowClass()
	{
		UnregisterClass(ae::C_BuildInfo::GetNameW(),  GetModuleHandle(NULL));
	}

	LRESULT CALLBACK C_WindowClass::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		C_Window_Win32* pWindow(reinterpret_cast<C_Window_Win32*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA)));
		return pWindow ? pWindow->WndProc(umsg, wparam, lparam) : ::DefWindowProc(hwnd, umsg, wparam, lparam);
	}

	C_WindowClass windowClass;
} // namespace windowclass {

// *****************************************************************************************************
// *****************************************************************************************************
void C_Window_Win32::DoModal()
{
	m_bDoModal = true;
	MSG msg;

	while(BOOL b = ::GetMessage( &msg, 0, 0, 0 ) != 0)
	{
		AE_ASSERT(b != -1 && "GetMessage return error code !");

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		if(!m_bDoModal)
			return;
	}
}

void C_Window_Win32::EndModal()
{
	m_bDoModal = false;
}

void C_Window_Win32::CloseWindow()
{
	::SendMessage(m_hWnd, WM_CLOSE, 0, 0);
}

// *****************************************************************************************************
// *****************************************************************************************************
void C_Window_Win32::EnableDragAndDrop()
{
	::ChangeWindowMessageFilter( WM_DROPFILES, MSGFLT_ADD);
	::ChangeWindowMessageFilter( WM_COPYDATA , MSGFLT_ADD);
	::ChangeWindowMessageFilter( 0x0049                 , MSGFLT_ADD);
	::ChangeWindowMessageFilterEx( m_hWnd, WM_DROPFILES	, MSGFLT_ALLOW, 0);
	::ChangeWindowMessageFilterEx( m_hWnd, WM_COPYDATA	, MSGFLT_ALLOW, 0);
	::ChangeWindowMessageFilterEx( m_hWnd, 0x0049		, MSGFLT_ALLOW, 0);
	::DragAcceptFiles(m_hWnd, true);
}

void C_Window_Win32::DisableDragAndDrop()
{
	::DragAcceptFiles(m_hWnd, false);
}
// *****************************************************************************************************
// *****************************************************************************************************
static const TCHAR* WindowTypeToClassName(T_WindowType type)
{
	switch(type)
	{
		case windowType_Button		: return L"BUTTON";
		case windowType_StaticText	: return L"STATIC";
		case windowType_ComboBox	: return L"COMBOBOX";
		case windowType_TreeView	: return L"SYSTREEVIEW32";
		case windowType_ListView	: return L"SYSLISTVIEW32";
		case windowType_EditBox		: return L"EDIT";
		case windowType_NumberEdit	: return L"NUMBEREDIT";
	};
	AE_ASSERT(0 && "Invalid widget type !");
	return L"";
}

static bool IsCustomClass(T_WindowType type)
{
	switch(type)
	{
		case windowType_NumberEdit : return true;
	}
	return false;
}

static DWORD WindowToWindow32Style(T_WindowType widgetType, T_WindowStyle widgetStyle)
{
	DWORD retStyle = 0;

	retStyle |= (widgetStyle & windowStyle_Visible)			? WS_VISIBLE			: 0;
	retStyle |= (widgetStyle & windowStyle_TabStop)			? WS_TABSTOP			: 0;
	retStyle |= (widgetStyle & windowStyle_Child)			? WS_CHILD				: 0;
	retStyle |= (widgetStyle & windowStyle_Border)			? WS_BORDER				: 0;
	retStyle |= (widgetStyle & windowStyle_HScroll)			? WS_HSCROLL			: 0;
	retStyle |= (widgetStyle & windowStyle_VScroll)			? WS_VSCROLL			: 0;

	retStyle |= (widgetStyle & windowStyle_TiledWindow)		? WS_TILEDWINDOW		: 0;
	retStyle |= (widgetStyle & windowStyle_ClipChildrens)	? WS_CLIPCHILDREN		: 0;
	retStyle |= (widgetStyle & windowStyle_ClipSiblings)	? WS_CLIPSIBLINGS		: 0;
	retStyle |= (widgetStyle & windowStyle_DlgFrame)		? WS_DLGFRAME			: 0;

	switch(widgetType)
	{
	case windowType_Button:
			retStyle |= (widgetStyle & windowStyle_PushButton)		? BS_DEFPUSHBUTTON		: 0;
			retStyle |= (widgetStyle & windowStyle_GroupBox)		? BS_GROUPBOX			: 0;
			retStyle |= (widgetStyle & windowStyle_AutoCheckBox)	? BS_AUTOCHECKBOX		: 0;
			retStyle |= (widgetStyle & windowStyle_AutoRadioBox)	? BS_AUTORADIOBUTTON	: 0;
			break;
	case windowType_StaticText: 
			break;
	case windowType_ComboBox:
			retStyle |= (widgetStyle & windowStyle_DropDown)		? CBS_DROPDOWN			: 0;
			retStyle |= (widgetStyle & windowStyle_Sorted)			? CBS_SORT				: 0;
			retStyle |= (widgetStyle & windowStyle_HasStrings)		? CBS_HASSTRINGS		: 0;
			retStyle |= (widgetStyle & windowStyle_LowerCase)		? CBS_LOWERCASE			: 0;
			retStyle |= (widgetStyle & windowStyle_UpperCase)		? CBS_UPPERCASE			: 0;
			retStyle |= (widgetStyle & windowStyle_AutoHScroll)		? CBS_AUTOHSCROLL		: 0;
			break;
	case windowType_TreeView:
			retStyle |= (widgetStyle & windowStyle_HasLines)		? TVS_HASLINES			: 0;
			retStyle |= (widgetStyle & windowStyle_HasButtons)		? TVS_HASBUTTONS		: 0;
			retStyle |= (widgetStyle & windowStyle_LinesAsRoot)		? TVS_LINESATROOT		: 0;
			retStyle |= (widgetStyle & windowStyle_EditLabels)		? TVS_EDITLABELS		: 0;
			retStyle |= (widgetStyle & windowStyle_TrackSelect)		? TVS_TRACKSELECT		: 0;
			retStyle |= (widgetStyle & windowStyle_SingleExpand)	? TVS_SINGLEEXPAND		: 0;
			retStyle |= (widgetStyle & windowStyle_FullRowSelect)	? TVS_FULLROWSELECT		: 0;
			retStyle |= (widgetStyle & windowStyle_InfoTip)			? TVS_INFOTIP			: 0;
			retStyle |= (widgetStyle & windowStyle_ShowSelAlways)   ? TVS_SHOWSELALWAYS		: 0;
			break;
	};

	return retStyle;
}

// *****************************************************************************************************
// *****************************************************************************************************
// Constructor for control's windows
C_Window_Win32::C_Window_Win32(C_Window_Win32* pParent, T_WindowType type, T_WindowStyle widgetStyle, T_ControlID controlID) :
	m_pParent(pParent),
	m_ControlID(controlID),
	m_WndProc(0),
	m_pBaseControl(0)
{
	m_hWnd = CreateWindowEx(0, WindowTypeToClassName(type), L"", WindowToWindow32Style(type, widgetStyle),
				0, 0, 100, 24, *pParent, (HMENU)controlID, GetModuleHandle(NULL), NULL);

	DefaultWindowInitialization();

	if(!IsCustomClass(type)) // store current WndProc only if it's not custom, otherwise we already have WndProc and we dont have to call it
	{
		// Store old control WndProc
		m_WndProc = (WNDPROC) ::GetWindowLongPtr(m_hWnd, GWLP_WNDPROC);
		 // Set our own WndProc only if not our own custom
		::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)windowclass::C_WindowClass::WndProc);
	}
}

// *****************************************************************************************************
C_Window_Win32::C_Window_Win32(T_WindowStyle widgetStyle) :
	m_pParent(0),
	m_ControlID(0),
	m_WndProc(0),
	m_pBaseControl(0)
{
	DWORD windowStyle(WindowToWindow32Style(0, widgetStyle));
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, ae::editor::C_BuildInfo::GetNameW(), ae::editor::C_BuildInfo::GetNameW(), windowStyle,
				0, 0, 600, 480, NULL, NULL, GetModuleHandle(NULL), NULL);

	DefaultWindowInitialization();
}

// *****************************************************************************************************
C_Window_Win32::C_Window_Win32(C_Window_Win32* pParent, T_WindowStyle widgetStyle) :
	m_pParent(pParent),
	m_ControlID(0),
	m_WndProc(0),
	m_pBaseControl(0)
{
	DWORD windowStyle(WindowToWindow32Style(0, widgetStyle));
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, ae::editor::C_BuildInfo::GetNameW(), ae::editor::C_BuildInfo::GetNameW(), windowStyle,
				0, 0, 600, 480, *pParent, NULL, GetModuleHandle(NULL), NULL);

	DefaultWindowInitialization();
}

// *****************************************************************************************************
void C_Window_Win32::DefaultWindowInitialization()
{
	SendMessage(m_hWnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), true);
	::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
	EnableDragAndDrop();
	UpdateWindow(m_hWnd); 
}

// *****************************************************************************************************
C_Window_Win32::~C_Window_Win32()
{
	DisableDragAndDrop();
	::DestroyWindow(m_hWnd);
}

// *****************************************************************************************************
// *****************************************************************************************************
LRESULT C_Window_Win32::WndProc(UINT umsg, WPARAM wparam, LPARAM lparam )
{
	S_WPReturn result(0);
	if(m_pBaseControl) result = m_pBaseControl->WndProc(umsg, wparam, lparam, result);

	switch(umsg)
	{
// *****************************************************************************************************
		case WM_TIMER:
			OnTimer((ae::u32)wparam);
			break;
// *****************************************************************************************************
		case WM_DESTROY : 
			OnDestroy();
			break;
// *****************************************************************************************************
		case WM_CLOSE : 
			OnClose();
			break;
// *****************************************************************************************************
		case WM_COMMAND :
			std::for_each(m_Controls.begin(), m_Controls.end(), [&](C_BaseCtrl_Win32* pControl) {
				if(pControl->GetControlID() == (T_ControlID)LOWORD(wparam))
					result = pControl->WndProc(umsg, wparam, lparam, result);
			});
			OnCommand(LOWORD(wparam));
			break;
// *****************************************************************************************************
		case WM_NOTIFY :
			{
				T_Controls::iterator i(m_Controls.begin());
				for(; i != m_Controls.end(); ++i) {
					if((*i)->GetControlID() == (T_ControlID)LOWORD(wparam)) {
						result = (*i)->WndProc(umsg, wparam, lparam, result);
						if(result.defaultWndProcCalled)	return result;
					}
				}
				break;
			}
// *****************************************************************************************************
		case WM_SIZE : 
			OnSize(LOWORD(lparam), HIWORD(lparam));
			break;
// *****************************************************************************************************
	}

	if(result.defaultWndProcCalled)
		return result;

	return CallDefaultWndProc(umsg, wparam, lparam);
}

LRESULT C_Window_Win32::CallDefaultWndProc(UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return m_WndProc ? m_WndProc(m_hWnd, umsg, wparam, lparam) : ::DefWindowProc(m_hWnd, umsg, wparam, lparam);
}

// *****************************************************************************************************
// *****************************************************************************************************
void C_Window_Win32::EnableWindow(bool bEnable)
{
	::EnableWindow(m_hWnd, bEnable);
}

bool C_Window_Win32::IsWindowEnabled() const
{
	return ::IsWindowEnabled(m_hWnd) ? true : false;
}
// *****************************************************************************************************

void C_Window_Win32::ShowWindow(bool bShow)
{
	::ShowWindow(m_hWnd, bShow ? SW_SHOW : SW_HIDE);
}

bool C_Window_Win32::IsVisible() const
{
	return ::IsWindowVisible(m_hWnd) ? true : false;
}
// *****************************************************************************************************

void C_Window_Win32::SetText(const char* sTitle)
{
	SetWindowTextA(m_hWnd, sTitle);
}

void C_Window_Win32::GetRect(S_Rect& rect, bool clientRect) const
{
	RECT _rect;
	clientRect ? ::GetClientRect(m_hWnd, &_rect) : ::GetWindowRect(m_hWnd, &_rect);

	rect.left = _rect.left;
	rect.right = _rect.right;
	rect.top = _rect.top;
	rect.bottom = _rect.bottom;
}

void C_Window_Win32::SetRect(const S_Rect& rect, bool /*clientRect*/)
{
	::SetWindowPos(m_hWnd, 0, rect.left, rect.top, rect.Width(), rect.Height(), 0 );
}

void C_Window_Win32::SetSize(ae::u32 width, ae::u32 height)
{
	::SetWindowPos(m_hWnd, 0, 0, 0, width, height, SWP_NOMOVE);
}

void C_Window_Win32::SetPos(ae::u32 x, ae::u32 y)
{
	::SetWindowPos(m_hWnd, 0, x, y, 0, 0, SWP_NOSIZE);
}

void C_Window_Win32::ClientToScreen(S_Point& point)
{
	POINT pt;
	pt.x = point.x;
	pt.y = point.y;
	::ClientToScreen(m_hWnd, &pt);
	point.x = pt.x;
	point.y = pt.y;
}

void C_Window_Win32::ScreenToClient(S_Point& point)
{
	POINT pt;
	pt.x = point.x;
	pt.y = point.y;
	::ScreenToClient(m_hWnd, &pt);
	point.x = pt.x;
	point.y = pt.y;
}

// *****************************************************************************************************
// *****************************************************************************************************

void C_Window_Win32::AddControl(C_BaseCtrl_Win32* pControl)
{
#if defined(AE_DEBUG) || defined(AE_DEVELOP)
	if(std::find_if(m_Controls.begin(), m_Controls.end(),[pControl](C_BaseCtrl_Win32* pcontrol) { return pControl->GetControlID() == pcontrol->GetControlID(); } ) != m_Controls.end())
	{
		AE_ASSERT(0 && "Control with duplicit control ID detected !");
	}
#endif // #if defined(AE_DEBUG) || defined(AE_DEVELOP)

	AE_ASSERT(std::find(m_Controls.begin(), m_Controls.end(), pControl) == m_Controls.end() && "Control already added !");
	m_Controls.push_back(pControl);
}

void C_Window_Win32::RemoveControl(C_BaseCtrl_Win32* pControl)
{
	T_Controls::iterator i(std::find(m_Controls.begin(), m_Controls.end(), pControl));
	AE_ASSERT(i != m_Controls.end() && "Control was not added !");
	m_Controls.erase(i);
}
// *****************************************************************************************************
// *****************************************************************************************************

std::string C_Window_Win32::GetText() const
{
	char buffer[::ae::maxStringSize];
	::GetWindowTextA(m_hWnd, buffer, ::ae::maxStringSize);
	return std::string(buffer);
}

} } } // namespace ae { namespace windows_sys {  namespace win32 {

#endif // #if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
