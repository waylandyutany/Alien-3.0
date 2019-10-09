#include "StdAfx.h"

#include "input_dxinput8.h"
#include "input_utils_dxinput8.h"

#include "../../debug_sys/output/outputs.h"
#include "../../_sys/templates.h"

namespace ae { namespace input { namespace dxinput8 {

C_InputSys::C_InputSys(HWND hWnd) :
	m_hMainWindow(hWnd),
	m_bIsFrozen(false)
{
	HRESULT result;

	// Buffered access mode property
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = 16; // Arbitrary buffer size

	result = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInput, NULL);

	result = m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_KeyboardDevice, NULL);
	result = m_KeyboardDevice->SetDataFormat(&c_dfDIKeyboard);	
	result = m_KeyboardDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
    result = m_KeyboardDevice->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
	result = m_KeyboardDevice->Acquire();	

	result = m_DirectInput->CreateDevice(GUID_SysMouse, &m_MouseDevice, NULL);
	result = m_MouseDevice->SetDataFormat(&c_dfDIMouse2);
	result = m_MouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
    result = m_MouseDevice->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
	result = m_MouseDevice->Acquire();

	memset(m_KeyboardState,0,sizeof(m_KeyboardState));
	memset(m_PreviousKeyboardState,0,sizeof(m_PreviousKeyboardState));

	memset(&m_MouseState,0,sizeof(m_MouseState));
	memset(&m_PreviousMouseState,0,sizeof(m_PreviousMouseState));
}

C_InputSys::~C_InputSys()
{
	if(m_MouseDevice)
	{
		m_MouseDevice->Unacquire();
		ae::SafeRelease(m_MouseDevice);
	}

	if(m_KeyboardDevice)
	{
		m_KeyboardDevice->Unacquire();
		ae::SafeRelease(m_KeyboardDevice);
	}

	ae::SafeRelease(m_DirectInput);
}

bool C_InputSys::HandleMsgPump()
{
	MSG msg;

	while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if(msg.hwnd == m_hMainWindow)
			ProcessMainWindowMessage(msg);

		if(msg.message == WM_QUIT)
			return false;
	}

	return true;
}

void C_InputSys::ProcessMainWindowMessage(MSG& msg)
{
	switch(msg.message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);		
		break;
	case WM_KEYDOWN:
		break;
 	case WM_KEYUP:
		if(msg.wParam == VK_ESCAPE)
			PostQuitMessage(0);		
		break;
	};
}

void C_InputSys::Update()
{
	ProcessMouse();
	ProcessKeyboard();
}

void C_InputSys::ProcessMouse()
{
	if(m_bIsFrozen)	{
		ZeroStruct<T_MouseState>(&m_MouseState);
		return;
	}

	HRESULT result;
	result = m_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&m_MouseState);
	if(result != DI_OK)	
		while(m_MouseDevice->Acquire() == DIERR_INPUTLOST);
	if(result != DI_OK && result != DIERR_INPUTLOST && result != DIERR_NOTACQUIRED)
		DBG_ERR << "MouseDevice->GetDeviceState failed !" << std::endl;

	for(ae::u32 i = 0; i < 3; ++i)
	{
		if((m_PreviousMouseState.rgbButtons[i] & 0x80) && !(m_MouseState.rgbButtons[i] & 0x80))
			m_MouseState.rgbButtons[i] |= 0x20; // released
		else if(!(m_PreviousMouseState.rgbButtons[i] & 0x80) && (m_MouseState.rgbButtons[i] & 0x80))
			m_MouseState.rgbButtons[i] |= 0x40; // pressed
	}

	memcpy(&m_PreviousMouseState, &m_MouseState, sizeof(m_MouseState));
}

void C_InputSys::ProcessKeyboard()
{
	if(m_bIsFrozen)	{
		ZeroStruct<T_KeyboardState>(&m_KeyboardState);
		return;
	}

	HRESULT result;
	result = m_KeyboardDevice->GetDeviceState(sizeof(m_KeyboardState), (LPVOID)&m_KeyboardState);
	if(result != DI_OK)	
		while(m_KeyboardDevice->Acquire() == DIERR_INPUTLOST);
	if(result != DI_OK && result != DIERR_INPUTLOST && result != DIERR_NOTACQUIRED)
		DBG_ERR << "KeyboardDevice->GetDeviceState failed !" << std::endl;

	for(ae::u32 i = 0; i < 256; ++i)
	{
		if((m_PreviousKeyboardState[i] & 0x80) && !(m_KeyboardState[i] & 0x80))
			m_KeyboardState[i] |= 0x20; // released
		else if(!(m_PreviousKeyboardState[i] & 0x80) && (m_KeyboardState[i] & 0x80))
			m_KeyboardState[i] |= 0x40; // pressed
	}

	memcpy(m_PreviousKeyboardState, m_KeyboardState, sizeof(m_KeyboardState));
}

bool C_InputSys::KeyState(E_Key_State keyState, E_Key_Code keyCode)
{
	ae::u32 keyIndex = static_cast<ae::u32>(keyCode);
	switch(keyState)
	{
		case E_KS_Key_Down		: return m_KeyboardState[keyIndex]		& 0x80	? true : false;
		case E_KS_Key_Up		: return !(m_KeyboardState[keyIndex]	& 0x80)	? true : false;
		case E_KS_Key_Pressed	: return m_KeyboardState[keyIndex]		& 0x40	? true : false;
		case E_KS_Key_Released	: return m_KeyboardState[keyIndex]		& 0x20	? true : false;
	}
	return false;
}

bool C_InputSys::MouseState(E_Key_State keyState, E_Mouse_Button mouseButton)
{
	switch(keyState)
	{
		case E_KS_Key_Down		: return (m_MouseState.rgbButtons[mouseButton]	& 0x80)	? true : false;
		case E_KS_Key_Up		: return !(m_MouseState.rgbButtons[mouseButton] & 0x80) ? true : false;
		case E_KS_Key_Pressed	: return (m_MouseState.rgbButtons[mouseButton]	& 0x40)	? true : false;
		case E_KS_Key_Released	: return (m_MouseState.rgbButtons[mouseButton]	& 0x20)	? true : false;
	}
	return false;
}

const char* C_InputSys::GetProviderName() const
{
	return "DirectInput8";
}

void C_InputSys::Quit()
{
	PostQuitMessage(0);
}

ae::math::float2 C_InputSys::MouseCursorPos() const
{
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(m_hMainWindow, &pt);
	return ae::math::float2((float)pt.x, (float)pt.y);
}

} } } // namespace ae { namespace input { namespace dxinput8 {
