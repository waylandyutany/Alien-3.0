#pragma once
#if !defined(AE_WIDGET_CONSTANTS_H) && defined(AE_EDITOR)
#define AE_WIDGET_CONSTANTS_H

namespace ae { namespace windows_sys {
// *****************************************************************************************************
	typedef ::ae::u32 T_WindowType;
	typedef ::ae::u32 T_WindowStyle;
	typedef ::ae::u32 T_MessageBoxReturnCode;
	typedef ::ae::u32 T_MessageBoxStyle;
// *****************************************************************************************************
// General window styles
	static T_WindowStyle windowStyle_Visible	= 1 << 0;
	static T_WindowStyle windowStyle_TabStop	= 1 << 1;
	static T_WindowStyle windowStyle_Child		= 1 << 2;
	static T_WindowStyle windowStyle_Border		= 1 << 3;
	static T_WindowStyle windowStyle_HScroll	= 1 << 4;
	static T_WindowStyle windowStyle_VScroll	= 1 << 5;
	static T_WindowStyle windowStyle_TiledWindow	= 1 << 6;
	static T_WindowStyle windowStyle_ClipChildrens 	= 1 << 7;
	static T_WindowStyle windowStyle_ClipSiblings 	= 1 << 8;
	static T_WindowStyle windowStyle_DlgFrame 		= 1 << 9;
	static T_WindowStyle widgetStyleMax			= 9;
// *****************************************************************************************************
// *****************************************************************************************************
	static const T_WindowType windowType_Button			= 1;
// *****************************************************************************************************
		static T_WindowStyle windowStyle_PushButton		= 1 << (widgetStyleMax + 1);
		static T_WindowStyle windowStyle_GroupBox		= 1 << (widgetStyleMax + 2);
		static T_WindowStyle windowStyle_AutoCheckBox	= 1 << (widgetStyleMax + 3);
		static T_WindowStyle windowStyle_AutoRadioBox	= 1 << (widgetStyleMax + 4);

// *****************************************************************************************************
// *****************************************************************************************************
	static const T_WindowType windowType_StaticText	= 2;

// *****************************************************************************************************
// *****************************************************************************************************
	static const T_WindowType windowType_ComboBox	= 3;
// *****************************************************************************************************
		static T_WindowStyle windowStyle_DropDown	= 1 << (widgetStyleMax + 1);
		static T_WindowStyle windowStyle_Sorted		= 1 << (widgetStyleMax + 2);
		static T_WindowStyle windowStyle_HasStrings	= 1 << (widgetStyleMax + 3);
		static T_WindowStyle windowStyle_LowerCase	= 1 << (widgetStyleMax + 4);
		static T_WindowStyle windowStyle_UpperCase	= 1 << (widgetStyleMax + 5);
		static T_WindowStyle windowStyle_AutoHScroll= 1 << (widgetStyleMax + 6);
// *****************************************************************************************************
// *****************************************************************************************************
	static const T_WindowType windowType_TreeView	= 4;
// *****************************************************************************************************
		static T_WindowStyle windowStyle_HasLines		= 1 << (widgetStyleMax + 1);
		static T_WindowStyle windowStyle_HasButtons		= 1 << (widgetStyleMax + 2);
		static T_WindowStyle windowStyle_LinesAsRoot	= 1 << (widgetStyleMax + 3);
		static T_WindowStyle windowStyle_EditLabels		= 1 << (widgetStyleMax + 4);
		static T_WindowStyle windowStyle_TrackSelect	= 1 << (widgetStyleMax + 5);
		static T_WindowStyle windowStyle_SingleExpand	= 1 << (widgetStyleMax + 6);
		static T_WindowStyle windowStyle_FullRowSelect	= 1 << (widgetStyleMax + 7);
		static T_WindowStyle windowStyle_InfoTip		= 1 << (widgetStyleMax + 8);
		static T_WindowStyle windowStyle_ShowSelAlways	= 1 << (widgetStyleMax + 9);
// *****************************************************************************************************
// *****************************************************************************************************
	static const T_WindowType windowType_ListView	= 5;
// *****************************************************************************************************

// *****************************************************************************************************
// *****************************************************************************************************
	static const T_WindowType windowType_EditBox	= 6;
// *****************************************************************************************************

// *****************************************************************************************************
	// *****************************************************************************************************
	// *****************************************************************************************************
	static const T_WindowType windowType_NumberEdit	= 7;
	// *****************************************************************************************************

// *****************************************************************************************************
static const T_MessageBoxReturnCode mb_ReturnCode_Abort		= 0; // Abort button was selected. 
static const T_MessageBoxReturnCode mb_ReturnCode_Cancel	= 1; // Cancel button was selected. 
static const T_MessageBoxReturnCode mb_ReturnCode_Continue	= 2; // Continue button was selected. 
static const T_MessageBoxReturnCode mb_ReturnCode_Ignore	= 3; // Ignore button was selected. 
static const T_MessageBoxReturnCode mb_ReturnCode_No		= 4; // No button was selected. 
static const T_MessageBoxReturnCode mb_ReturnCode_OK		= 5; // OK button was selected. 
static const T_MessageBoxReturnCode mb_ReturnCode_Retry		= 6; // Retry button was selected. 
static const T_MessageBoxReturnCode mb_ReturnCode_TryAgain	= 7; // Try Again button was selected. 
static const T_MessageBoxReturnCode mb_ReturnCode_Yes		= 8; // Yes button was selected. 

static const T_MessageBoxStyle mb_Style_AbortRetryIgnore	= 1 << 0;
static const T_MessageBoxStyle mb_Style_CancelTryContinue	= 1 << 1;
static const T_MessageBoxStyle mb_Style_Help				= 1 << 2;
static const T_MessageBoxStyle mb_Style_OK					= 1 << 3;
static const T_MessageBoxStyle mb_Style_OKCancel			= 1 << 4;
static const T_MessageBoxStyle mb_Style_RetryCancel			= 1 << 5;
static const T_MessageBoxStyle mb_Style_YesNo				= 1 << 6;
static const T_MessageBoxStyle mb_Style_YesNoCancel			= 1 << 7;
static const T_MessageBoxStyle mb_Style_IconExclamation		= 1 << 8;
static const T_MessageBoxStyle mb_Style_IconWarning			= 1 << 9;
static const T_MessageBoxStyle mb_Style_IconInformation		= 1 << 10;
static const T_MessageBoxStyle mb_Style_IconAsterisk		= 1 << 11;
static const T_MessageBoxStyle mb_Style_IconQuestion		= 1 << 12;
static const T_MessageBoxStyle mb_Style_IconStop			= 1 << 13;
static const T_MessageBoxStyle mb_Style_IconError			= 1 << 14;
static const T_MessageBoxStyle mb_Style_IconHand			= 1 << 15;
static const T_MessageBoxStyle mb_Style_DeffButton1			= 1 << 16;
static const T_MessageBoxStyle mb_Style_DeffButton2			= 1 << 17;
static const T_MessageBoxStyle mb_Style_DeffButton3			= 1 << 18;
static const T_MessageBoxStyle mb_Style_DeffButton4			= 1 << 19;
static const T_MessageBoxStyle mb_Style_AppModal			= 1 << 20;
static const T_MessageBoxStyle mb_Style_SystemModal			= 1 << 21;
static const T_MessageBoxStyle mb_Style_TaskModal			= 1 << 22;
static const T_MessageBoxStyle mb_Style_DefaultDesktopOnly	= 1 << 23;
static const T_MessageBoxStyle mb_Style_Right				= 1 << 24;
static const T_MessageBoxStyle mb_Style_RtlReading			= 1 << 25;
static const T_MessageBoxStyle mb_Style_SetForeground		= 1 << 26;
static const T_MessageBoxStyle mb_Style_TopMost				= 1 << 27;

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_WIDGET_CONSTANTS_H) && defined(AE_EDITOR)
