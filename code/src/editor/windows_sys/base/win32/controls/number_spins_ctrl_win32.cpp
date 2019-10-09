#include "stdafx.h"

#if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))

#include "number_spins_ctrl_win32.h"
#include "../../../widgets/number_spins.h"

namespace ae { namespace windows_sys {  namespace win32 {

// *****************************************************************************************************
// *****************************************************************************************************
	namespace {

		class C_NumberEditClass
		{
			WNDCLASSEX windowClass;
		public:
			C_NumberEditClass();
			~C_NumberEditClass();

			static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

			HPEN penLightGray, penDarkDarkGray, penDarkGray, penBlack, penWhite;
			HCURSOR normalCursor, dragCursor;
		};

		C_NumberEditClass::C_NumberEditClass()
		{
			windowClass.style			= CS_HREDRAW | CS_VREDRAW;;
			windowClass.lpfnWndProc		= WndProc;
			windowClass.cbClsExtra		= 0;
			windowClass.cbWndExtra		= 0;
			windowClass.hInstance		= GetModuleHandle(NULL);
			windowClass.hIcon			= 0;
			windowClass.hIconSm			= 0;
			windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
			windowClass.hbrBackground	= (HBRUSH)COLOR_WINDOW;//(HBRUSH)GetStockObject(BLACK_BRUSH);
//			windowClass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
			windowClass.lpszMenuName	= NULL;
			windowClass.lpszClassName	= L"NUMBEREDIT";
			windowClass.cbSize			= sizeof(WNDCLASSEX);

			if(!RegisterClassEx(&windowClass))
			{
				AE_ASSERT(0 && "Unable to register 'NUMBEREDIT' window class !");
			}

			penLightGray = CreatePen(PS_SOLID, 0, RGB(192, 192, 192));
			penDarkGray = CreatePen(PS_SOLID, 0, RGB(128, 128, 128));
			penDarkDarkGray = CreatePen(PS_SOLID, 0, RGB(64, 64, 64));
			penBlack = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
			penWhite = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

			normalCursor = LoadCursor(NULL, IDC_ARROW);
			dragCursor = LoadCursor(NULL, IDC_SIZENS);
		}

		C_NumberEditClass::~C_NumberEditClass()
		{
			DeleteObject(penLightGray);
			DeleteObject(penDarkGray);
			DeleteObject(penDarkDarkGray);
			DeleteObject(penBlack);
			DeleteObject(penWhite);

			UnregisterClass(ae::C_BuildInfo::GetNameW(),  GetModuleHandle(NULL));
		}

		LRESULT CALLBACK C_NumberEditClass::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
		{
			C_Window_Win32* pWindow(reinterpret_cast<C_Window_Win32*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA)));
			return pWindow ? pWindow->WndProc(umsg, wparam, lparam) : ::DefWindowProc(hwnd, umsg, wparam, lparam);
		}

		C_NumberEditClass numberEditWindowClass;
	} // namespace numberedit_windowclass {

// *****************************************************************************************************
// *****************************************************************************************************

C_NumberSpinCtrl_Win32::C_NumberSpinCtrl_Win32(ae::windows_sys::E_NumberSpinType type, C_NumberSpinCallback* pCallback, C_Window_Win32& control) :
	params(*this, type),
	pCallback(pCallback), 
	C_BaseCtrl_Win32(control),
	state(E_None),
	timeToSinglePress(500), timeToAutoPress(60),
	wheelDeltaBoost(10)
{
	editBoxHWND = ::CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL,
		0, 0, 40, 20, control, (HMENU)100, GetModuleHandle(NULL), NULL);
	SendMessage(editBoxHWND, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), true);
	UpdateWindow(m_Control); 

}

C_NumberSpinCtrl_Win32::~C_NumberSpinCtrl_Win32()
{
	::DestroyWindow(editBoxHWND);
}

// *****************************************************************************************************
bool C_NumberSpinCtrl_Win32::IsMousePressed() const
{
	if(state == E_MousePressedPlus || state == E_MousePressedMinus || state == E_MousePressedBoth)
		return true;
	return false;
}

void C_NumberSpinCtrl_Win32::UpdateMouseCursor()
{
	if(state == E_MousePressedBoth || state == E_MouseOverBoth)
		SetCursor(numberEditWindowClass.dragCursor);
	else
		SetCursor(numberEditWindowClass.normalCursor);
}

void C_NumberSpinCtrl_Win32::UpdateMouseState(POINT& mousePos, bool bPressed)
{
	RECT dragRect, plusRect, minusRect;
	CalculateRectangles(0, &plusRect, &minusRect, &dragRect);

	if(PtInRect(&plusRect, mousePos)) {
		state = bPressed ? E_MousePressedPlus : E_MouseOverPlus;
	}
	else if(PtInRect(&minusRect, mousePos)) {
		state = bPressed ? E_MousePressedMinus : E_MouseOverMinus;
	}
	else if(PtInRect(&dragRect, mousePos)) {
		state = bPressed ? E_MousePressedBoth : E_MouseOverBoth;
	}
	else if(!bPressed) {
		state = E_None;
	}
}

void C_NumberSpinCtrl_Win32::UpdateMouseState(bool bPressed)
{
	if(bPressed)
	{
		if(state == E_MouseOverPlus)
			state = E_MousePressedPlus;
		else if(state == E_MouseOverMinus)
			state = E_MousePressedMinus;
		else if(state == E_MouseOverBoth)
			state = E_MousePressedBoth;
		else {
			AE_ASSERT(0 && "Must be in some mouse over state !");
		}
	}
	else
	{
		if(state == E_MousePressedPlus)
			state = E_MouseOverPlus;
		else if(state == E_MousePressedMinus)
			state = E_MouseOverMinus;
		else if(state == E_MousePressedBoth)
			state = E_MouseOverBoth;
		else {
			AE_ASSERT(0 && "Must be in some mouse pressed state !");
		}
	}
}

void C_NumberSpinCtrl_Win32::NotityPlusMinus()
{
	if(state == E_MousePressedPlus)
		params.ChangeValue(1); 
	else if(state == E_MousePressedMinus)
		params.ChangeValue(-1); 
}

// *****************************************************************************************************
/*virtual*/ S_WPReturn C_NumberSpinCtrl_Win32::WndProc(UINT umsg, WPARAM wparam, LPARAM lparam, S_WPReturn result)
{
	switch(umsg)
	{
	case WM_SIZE:
		{
			clientRect.right	= LOWORD(lparam);
			clientRect.bottom	= HIWORD(lparam);
			clientRect.left		= clientRect.top = 0;

			RECT editRect;
			CalculateRectangles(&editRect,0,0,0);
			SetWindowPos(editBoxHWND, 0, editRect.left, editRect.top, editRect.right - editRect.left, editRect.bottom - editRect.top, 0);
			::InvalidateRect(m_Control, 0, true);
			break;
		}
	case WM_PAINT:
		{
			PAINTSTRUCT ps; 
			HDC hdc;
			hdc = BeginPaint(m_Control, &ps);
			DrawSmartSpin(hdc);
			EndPaint(m_Control, &ps);
			return 0;
		}
	case WM_MOUSEMOVE:
		{
			POINT mousePos;

			mousePos.x = LOWORD(lparam);
			mousePos.y = HIWORD(lparam);

			if(!IsMousePressed()) {
				UpdateMouseState(mousePos, false);
			}
			else if(state == E_MousePressedBoth)
			{
				GetCursorPos(&mousePos);
				params.ChangeValue(mouseDragPos.y - mousePos.y);
				mouseDragPos = mousePos;
			}

			::InvalidateRect(m_Control, 0, true);
			UpdateMouseCursor();
			break;
		}
	case WM_LBUTTONDOWN:
		{
			AE_ASSERT(!IsMousePressed() && "Must not be pressed state !");

			GetCursorPos(&mouseDragPos);

			UpdateMouseState(true);
			NotityPlusMinus();
			SetCapture(m_Control);
			UpdateMouseCursor();
			SetTimer(m_Control, 1, timeToSinglePress, 0);
			::InvalidateRect(m_Control, 0, true);
			break;
		}
	case WM_LBUTTONUP:
		{
			AE_ASSERT(IsMousePressed() && "Must be mouse pressed state !");

			UpdateMouseState(false);
			NotityPlusMinus();
			ReleaseCapture();
			UpdateMouseCursor();
			KillTimer(m_Control, 1);
			::InvalidateRect(m_Control, 0, true);
			break;
		}
	case WM_TIMER:
		{
			NotityPlusMinus();
			SetTimer(m_Control, 1, timeToAutoPress, 0);
			break;
		}
	case WM_MOUSEWHEEL:
		{
			long zDelta = GET_WHEEL_DELTA_WPARAM(wparam) / WHEEL_DELTA;
			params.ChangeValue(zDelta * wheelDeltaBoost);
			return 0;
		}
	case WM_COMMAND:
		{
			if(HIWORD(wparam) == EN_CHANGE)
			{
				char buffer[32];
				::GetWindowTextA(editBoxHWND, buffer, 32);
				params.SetValue(buffer);
			}
			break;
		}
	case WM_ENABLE:
		InvalidateRect(m_Control,0,true);
		break;
	}
	if(!pCallback) return result;

	return result;
}

void C_NumberSpinCtrl_Win32::CalculateRectangles(RECT* editRect, RECT* plusRect, RECT* minusRect, RECT* dragRect)
{
	const long spinWidth = 14;
	if(editRect)
	{
		editRect->left = clientRect.left;
		editRect->top = clientRect.top;
		editRect->right = clientRect.right - spinWidth;
		editRect->bottom = clientRect.bottom;
	}
	if(plusRect)
	{
		plusRect->left = clientRect.right - spinWidth;
		plusRect->top = clientRect.top - 1;
		plusRect->right = clientRect.right;
		plusRect->bottom = ((clientRect.bottom - clientRect.top) >> 1) - 1;
	}
	if(minusRect)
	{
		minusRect->left = clientRect.right - spinWidth;
		minusRect->top = ((clientRect.bottom - clientRect.top) >> 1);
		minusRect->right = clientRect.right;
		minusRect->bottom = clientRect.bottom;
	}
	if(dragRect)
	{
		dragRect->left = clientRect.right - spinWidth;
		dragRect->top = ((clientRect.bottom - clientRect.top) >> 1) - 2;
		dragRect->right = clientRect.right;
		dragRect->bottom = ((clientRect.bottom - clientRect.top) >> 1) + 2;
	}
}

 void C_NumberSpinCtrl_Win32::DrawSpinWindow(HDC hdc, RECT rect, HPEN ltPen, HPEN rbPen, HPEN bdPen)
{
	// border rectangle
	SelectObject(hdc, bdPen);
	MoveToEx(hdc, rect.left, rect.top, 0);
	LineTo(hdc, rect.right, rect.top);
	LineTo(hdc, rect.right, rect.bottom);
	LineTo(hdc, rect.left, rect.bottom);
	LineTo(hdc, rect.left, rect.top);

	// rect inflation
	rect.left += 1;
	rect.top += 1;
	rect.right -= 1;
	rect.bottom -= 1;

	// left and top edge
	SelectObject(hdc, ltPen);
	MoveToEx(hdc, rect.left, rect.bottom, 0);
	LineTo(hdc, rect.left, rect.top);
	LineTo(hdc, rect.right, rect.top);

	// right and bottom edge
	SelectObject(hdc, rbPen);
	MoveToEx(hdc, rect.right, rect.top, 0);
	LineTo(hdc, rect.right, rect.bottom);
	LineTo(hdc, rect.left, rect.bottom);
}

void C_NumberSpinCtrl_Win32::DrawMinusSpinArrow(HDC hdc, RECT rect, HPEN pen)
{
	SelectObject(hdc, pen);

	for(int i = 0; i < 4; i++) {
		MoveToEx(hdc, rect.left + i, rect.top + i, 0);
		LineTo(hdc, rect.right - i, rect.top + i);
	}
}

void C_NumberSpinCtrl_Win32::DrawPlusSpinArrow(HDC hdc, RECT rect, HPEN pen)
{
	SelectObject(hdc, pen);

	for(int i = 0; i < 4; i++) {
		MoveToEx(hdc, rect.left + i, rect.bottom - i, 0);
		LineTo(hdc, rect.right - i, rect.bottom - i);
	}
}

void C_NumberSpinCtrl_Win32::DrawSpin(HDC hdc, RECT rect, bool isPressed, bool isSelected, bool isMinus)
{
	DrawSpinWindow(hdc, rect,	isPressed ? numberEditWindowClass.penDarkGray : numberEditWindowClass.penWhite,
								isPressed ? numberEditWindowClass.penWhite : numberEditWindowClass.penDarkGray, 
								numberEditWindowClass.penDarkDarkGray);

	rect.left += 3;	rect.top += 3; rect.right -= 3; rect.bottom -= 3;

	HPEN arrowPen = ::IsWindowEnabled(m_Control) ? numberEditWindowClass.penBlack : numberEditWindowClass.penDarkGray;
	isMinus ? DrawMinusSpinArrow(hdc, rect, arrowPen) : DrawPlusSpinArrow(hdc, rect, arrowPen);
}

void C_NumberSpinCtrl_Win32::DrawSmartSpin(HDC hdc)
{
	RECT plusRect, minusRect;
	CalculateRectangles(0, &plusRect, &minusRect, 0);

	bool isMinusSelected = (state == E_MouseOverMinus) || (state == E_MousePressedMinus);
	bool isPlusSelected = (state == E_MouseOverPlus) || (state == E_MousePressedPlus);
	if((state == E_MouseOverBoth) || (state == E_MousePressedBoth)) {
		isMinusSelected = isPlusSelected = true;
	}

	bool isMinusPressed = ((state == E_MousePressedMinus) || (state == E_MousePressedBoth)) ? true : false;
	bool isPlusPressed = ((state == E_MousePressedPlus) || (state == E_MousePressedBoth)) ? true : false;

	DrawSpin(hdc, plusRect, isPlusPressed, isPlusSelected, false);
	DrawSpin(hdc, minusRect, isMinusPressed, isMinusSelected, true);
}

// *****************************************************************************************************
// *****************************************************************************************************
C_NumberSpinCtrl_Win32::S_Params::S_Params(C_NumberSpinCtrl_Win32& control, ae::windows_sys::E_NumberSpinType type) : 
	control(control),
	type(type),
	bLoopValue(false),
	floatPrecision(4)
{
	switch(type) {
	case E_NST_Float:
		minFloat = -1000000.0f;
		maxFloat =  1000000.0f;
		stepFloat = 0.01f;
		valueFloat = 1.0f;
		SetValueFloat(0.0f);
		break;
	case E_NST_Integer:
		minInteger = -1000000;
		maxInteger =  1000000;
		stepInteger = 1;
		valueInteger = 1;
		SetValueInteger(0);
		break;
	default:
		AE_ASSERT(0 && "Invalid number type !");
	}
}


void C_NumberSpinCtrl_Win32::S_Params::ChangeValue(long delta)
{
	switch(type) {
	case E_NST_Float :
		SetValueFloat(GetValueFloat() + (delta * stepFloat), true);
		break;
	case E_NST_Integer :
		SetValueInteger(GetValueInteger() + (delta * stepInteger), true);
		break;
	default :
		AE_ASSERT(0 && "Invalid number type !");
	}
}

void C_NumberSpinCtrl_Win32::S_Params::SetValueFloat(float value, bool notify, bool updateText)
{
	if(value < minFloat)
		value = bLoopValue ? maxFloat : minFloat;
	if(value > maxFloat)
		value = bLoopValue ? minFloat : maxFloat;

	if(valueFloat != value)
	{
		valueFloat = value;
		if(notify && control.pCallback) control.pCallback->OnFloatChange(valueFloat, control.GetControlID());
	}

	if(updateText) {
		std::stringstream ss;
		//	ss << std::showpos;
		ss << std::showpoint;
		ss << std::fixed;
		ss << std::setprecision(floatPrecision);
		ss << valueFloat;
		::SetWindowTextA(control.editBoxHWND, ss.str().c_str());
	}
}

void C_NumberSpinCtrl_Win32::S_Params::SetValueInteger(T_Integer value, bool notify, bool updateText)
{
	if(value < minInteger)
		value = bLoopValue ? maxInteger : minInteger;
	if(value > maxInteger)
		value = bLoopValue ? minInteger : maxInteger;

	if(valueInteger != value)
	{
		valueInteger = value;
		if(notify && control.pCallback) control.pCallback->OnIntegerChange(valueInteger, control.GetControlID());
	}
	if(updateText) {
		std::stringstream ss;
		ss << valueInteger;
		::SetWindowTextA(control.editBoxHWND, ss.str().c_str());
	}
}

void C_NumberSpinCtrl_Win32::S_Params::SetValue(const char* sValue)
{
	switch(type) {
	case E_NST_Float :
		SetValueFloat((float)::atof(sValue), true, false);
		break;
	case E_NST_Integer :
		SetValueInteger(::atoi(sValue), true, false);
		break;
	default :
		AE_ASSERT(0 && "Invalid number type !");
	}
}

// *****************************************************************************************************
// *****************************************************************************************************

} } } // namespace ae { namespace windows_sys {  namespace win32 {

#endif // #if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
