#include "stdafx.h"

#if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))

#include "message_box_ctrl_win32.h"
#include "../../win32/window_win32.h"

namespace ae { namespace windows_sys {  namespace win32 {

UINT MBStyle2Win32MBType(T_MessageBoxStyle style)
{
	UINT ret = 0;
	ret |= (style & mb_Style_AbortRetryIgnore)	? MB_ABORTRETRYIGNORE	: 0;
	ret |= (style & mb_Style_CancelTryContinue)	? MB_CANCELTRYCONTINUE	: 0;
	ret |= (style & mb_Style_Help)				? MB_HELP				: 0;
	ret |= (style & mb_Style_OK)				? MB_OK					: 0;
	ret |= (style & mb_Style_OKCancel)			? MB_OKCANCEL			: 0;
	ret |= (style & mb_Style_RetryCancel)		? MB_RETRYCANCEL		: 0;
	ret |= (style & mb_Style_YesNo)				? MB_YESNO				: 0;
	ret |= (style & mb_Style_YesNoCancel)		? MB_YESNOCANCEL		: 0;
	ret |= (style & mb_Style_IconExclamation)	? MB_ICONEXCLAMATION	: 0;
	ret |= (style & mb_Style_IconWarning)		? MB_ICONWARNING		: 0;
	ret |= (style & mb_Style_IconInformation)	? MB_ICONINFORMATION	: 0;
	ret |= (style & mb_Style_IconAsterisk)		? MB_ICONASTERISK		: 0;
	ret |= (style & mb_Style_IconQuestion)		? MB_ICONQUESTION		: 0;
	ret |= (style & mb_Style_IconStop)			? MB_ICONSTOP			: 0;
	ret |= (style & mb_Style_IconError)			? MB_ICONERROR			: 0;
	ret |= (style & mb_Style_IconHand)			? MB_ICONHAND			: 0;
	ret |= (style & mb_Style_DeffButton1)		? MB_DEFBUTTON1			: 0;
	ret |= (style & mb_Style_DeffButton2)		? MB_DEFBUTTON2			: 0;
	ret |= (style & mb_Style_DeffButton3)		? MB_DEFBUTTON3			: 0;
	ret |= (style & mb_Style_DeffButton4)		? MB_DEFBUTTON4			: 0;
	ret |= (style & mb_Style_AppModal)			? MB_APPLMODAL			: 0;
	ret |= (style & mb_Style_SystemModal)		? MB_SYSTEMMODAL		: 0;
	ret |= (style & mb_Style_TaskModal)			? MB_TASKMODAL			: 0;
	ret |= (style & mb_Style_DefaultDesktopOnly)? MB_DEFAULT_DESKTOP_ONLY: 0;
	ret |= (style & mb_Style_Right)				? MB_RIGHT				: 0;
	ret |= (style & mb_Style_RtlReading)		? MB_RTLREADING			: 0;
	ret |= (style & mb_Style_SetForeground)		? MB_SETFOREGROUND		: 0;
	ret |= (style & mb_Style_TopMost)			? MB_TOPMOST			: 0;
											   
	return ret;
}

T_MessageBoxReturnCode MBWin32ReturnCode2MBReturnCode(int retCode)
{
	switch(retCode)
	{
		case IDABORT	: return mb_ReturnCode_Abort;
		case IDCANCEL	: return mb_ReturnCode_Cancel;
		case IDCONTINUE	: return mb_ReturnCode_Continue;
		case IDIGNORE	: return mb_ReturnCode_Ignore;
		case IDNO		: return mb_ReturnCode_No;
		case IDOK		: return mb_ReturnCode_OK;
		case IDRETRY	: return mb_ReturnCode_Retry;
		case IDTRYAGAIN	: return mb_ReturnCode_TryAgain;
		case IDYES		: return mb_ReturnCode_Yes;
	}

	return 0;
}

T_MessageBoxReturnCode C_MessageBoxCtrl_Win32::Show(C_Window_Win32* pParent, T_MessageBoxStyle style, const char* sTitle, const char* sText)
{
	HWND hWnd(pParent ? pParent->GetHWnd() : 0);
	int ret = ::MessageBoxA(hWnd, sText, sTitle, MBStyle2Win32MBType(style));
	return MBWin32ReturnCode2MBReturnCode(ret);
}

} } } // namespace ae { namespace windows_sys {  namespace win32 {

#endif // #if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
