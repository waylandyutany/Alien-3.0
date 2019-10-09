#include "StdAfx.h"

#include "render_window_dx11.h"
#include "../../../input_sys/input_window_callback.h"

namespace ae { namespace render { namespace dx11 {

namespace window {
	
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
		windowClass.style			= CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc		= WndProc;
		windowClass.cbClsExtra		= 0;
		windowClass.cbWndExtra		= 0;
		windowClass.hInstance		= GetModuleHandle(NULL);
		windowClass.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
		windowClass.hIconSm			= windowClass.hIcon;
		windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
		windowClass.lpszMenuName	= NULL;
		windowClass.lpszClassName	= ae::C_BuildInfo::GetNameW();
		windowClass.cbSize			= sizeof(WNDCLASSEX);
	
		RegisterClassEx(&windowClass);
	}

	C_WindowClass::~C_WindowClass()
	{
		UnregisterClass(ae::C_BuildInfo::GetNameW(),  GetModuleHandle(NULL));
	}

	LRESULT CALLBACK C_WindowClass::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		LRESULT result = DefWindowProc(hwnd, umsg, wparam, lparam);
		ae::input::I_WindowCallback* pWindowCallback(reinterpret_cast<ae::input::I_WindowCallback*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA)));

		switch(umsg)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			if(pWindowCallback)
				pWindowCallback->OnMouseClick();
			break;
		case WM_EXITSIZEMOVE:
			if(pWindowCallback && wparam ==  SC_SIZE)
				pWindowCallback->OnResizeEnd();
			break;
		case WM_ENTERSIZEMOVE:
			if(pWindowCallback && wparam ==  SC_SIZE)
				pWindowCallback->OnResizeStart();
			break;
		case WM_SETFOCUS:
			if(pWindowCallback)
				pWindowCallback->OnFocusSet();
			break;
		case WM_KILLFOCUS:
			if(pWindowCallback)
				pWindowCallback->OnFocusLost();
			break;
		case WM_SIZE:
			if(pWindowCallback)
			{
				ae::u32 windowWidth(LOWORD(lparam));
				ae::u32 windowHeight(HIWORD(lparam));
				pWindowCallback->OnResize(windowWidth, windowHeight);
			}
			break;
		}
		return result;
	}

	C_WindowClass windowClass;
}

C_RenderWindow::C_RenderWindow() :
	m_pWindowCallback(0)
{
	DWORD windowStyle(WS_TILEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, ae::C_BuildInfo::GetNameW(), ae::C_BuildInfo::GetNameW(), windowStyle,
				0, 0, 600, 480, NULL, NULL, GetModuleHandle(NULL), NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hWnd, SW_SHOW);
	SetWindowText(m_hWnd, ae::C_BuildInfo::GetNameW());
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
}

C_RenderWindow::~C_RenderWindow()
{
	DestroyWindow(m_hWnd);
}

void C_RenderWindow::RegisterWindowCallback(ae::input::I_WindowCallback* pWindowCallback)
{
	AE_ASSERT(!m_pWindowCallback && "Only one window callback is supported atm !");
	m_pWindowCallback = pWindowCallback;
	::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG)m_pWindowCallback);
}

void C_RenderWindow::UnregisterWindowCallback(ae::input::I_WindowCallback* pWindowCallback)
{
	AE_ASSERT(m_pWindowCallback == pWindowCallback && "Only one window callback is supported atm !");
	m_pWindowCallback = pWindowCallback = 0;
	::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG)m_pWindowCallback);
}

void C_RenderWindow::SetWindowDimensions(ae::u32 windowWidth, ae::u32 windowHeight)
{
	LONG_PTR dwStyle = ::GetWindowLongPtr(m_hWnd, GWL_STYLE);
	LONG_PTR dwExStyle = ::GetWindowLongPtr(m_hWnd, GWL_EXSTYLE);
	HMENU menu = GetMenu(m_hWnd);
	RECT rc = { 0, 0, windowWidth, windowHeight } ;
	AdjustWindowRectEx( &rc, (DWORD)dwStyle, menu ? TRUE : FALSE, (DWORD)dwExStyle );
	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
}

void C_RenderWindow::GetWindowDimensions(ae::u32 windowWidth, ae::u32 windowHeight) const
{
	RECT clientRect;
	::GetClientRect(m_hWnd, &clientRect);
	windowWidth = clientRect.right - clientRect.left;
	windowHeight = clientRect.bottom - clientRect.top;
}

// *****************************************************************************************************

void C_RenderWindow::HideAndLockCursor(bool bHideAndLock)
{
	if(bHideAndLock)
	{
		while(::ShowCursor(false) >= 0);

		::RECT windowRect;
		::POINT lockPoint;
		::GetClientRect(m_hWnd, &windowRect);

		lockPoint.x = (windowRect.right - windowRect.left) >> 1;
		lockPoint.y = (windowRect.bottom - windowRect.top) >> 1;
		::ClientToScreen(m_hWnd, &lockPoint);
		::SetCursorPos(lockPoint.x, lockPoint.y);

		windowRect.left = lockPoint.x;
		windowRect.top = lockPoint.y;
		windowRect.right = lockPoint.x + 1;
		windowRect.bottom = lockPoint.y + 1;
		::ClipCursor(&windowRect);
	}
	else
	{
		while(::ShowCursor(true) < 0);

		::ClipCursor(0);
	}
}

bool C_RenderWindow::IsCursorHiddenAndLocked() const
{
	::RECT clipRect;
	::GetClipCursor(&clipRect);
	return clipRect.right - clipRect.left > 2 ? false : true;
}

// *****************************************************************************************************

} } } // namespace ae { namespace render { namespace dx11 {
