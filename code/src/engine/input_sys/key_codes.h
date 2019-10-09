#pragma once

#ifndef AE_INPUT_SYS_KEY_CODES_H
#define AE_INPUT_SYS_KEY_CODES_H

namespace ae { namespace input {

	enum E_Key_State
	{
		E_KS_Key_Down		= 0,
		E_KS_Key_Up			= 1,
		E_KS_Key_Pressed	= 2,
		E_KS_Key_Released	= 3,
	};

	enum E_Mouse_Button
	{
		E_MB_LEFT			= 0,
		E_MB_RIGHT			= 1,
		E_MB_MIDDLE			= 2
	};

	enum E_Key_Code
	{
		E_KC_ESCAPE          = 0x01,
		E_KC_1               = 0x02,
		E_KC_2               = 0x03,
		E_KC_3               = 0x04,
		E_KC_4               = 0x05,
		E_KC_5               = 0x06,
		E_KC_6               = 0x07,
		E_KC_7               = 0x08,
		E_KC_8               = 0x09,
		E_KC_9               = 0x0A,
		E_KC_0               = 0x0B,
		E_KC_MINUS           = 0x0C,   /* - on main keyboard */
		E_KC_EQUALS          = 0x0D,
		E_KC_BACK            = 0x0E,    /* backspace */
		E_KC_TAB             = 0x0F,
		E_KC_Q               = 0x10,
		E_KC_W               = 0x11,
		E_KC_E               = 0x12,
		E_KC_R               = 0x13,
		E_KC_T               = 0x14,
		E_KC_Y               = 0x15,
		E_KC_U               = 0x16,
		E_KC_I               = 0x17,
		E_KC_O               = 0x18,
		E_KC_P               = 0x19,
		E_KC_LBRACKET        = 0x1A,
		E_KC_RBRACKET        = 0x1B,
		E_KC_ENTER           = 0x1C,    /* Enter on main keyboard */
		E_KC_LCONTROL        = 0x1D,
		E_KC_A               = 0x1E,
		E_KC_S               = 0x1F,
		E_KC_D               = 0x20,
		E_KC_F               = 0x21,
		E_KC_G               = 0x22,
		E_KC_H               = 0x23,
		E_KC_J               = 0x24,
		E_KC_K               = 0x25,
		E_KC_L               = 0x26,
		E_KC_SEMICOLON       = 0x27,
		E_KC_APOSTROPHE      = 0x28,
		E_KC_GRAVE           = 0x29,   /* accent grave */
		E_KC_LSHIFT          = 0x2A,
		E_KC_BACKSLASH       = 0x2B,
		E_KC_Z               = 0x2C,
		E_KC_X               = 0x2D,
		E_KC_C               = 0x2E,
		E_KC_V               = 0x2F,
		E_KC_B               = 0x30,
		E_KC_N               = 0x31,
		E_KC_M               = 0x32,
		E_KC_COMMA           = 0x33,
		E_KC_PERIOD          = 0x34,    /* . on main keyboard */
		E_KC_SLASH           = 0x35,    /* / on main keyboard */
		E_KC_RSHIFT          = 0x36,
		E_KC_MULTIPLY        = 0x37,    /* * on numeric keypad */
		E_KC_LALT            = 0x38,    /* left Alt */
		E_KC_SPACE           = 0x39,
		E_KC_CAPITAL         = 0x3A,
		E_KC_F1              = 0x3B,
		E_KC_F2              = 0x3C,
		E_KC_F3              = 0x3D,
		E_KC_F4              = 0x3E,
		E_KC_F5              = 0x3F,
		E_KC_F6              = 0x40,
		E_KC_F7              = 0x41,
		E_KC_F8              = 0x42,
		E_KC_F9              = 0x43,
		E_KC_F10             = 0x44,
		E_KC_NUMLOCK         = 0x45,
		E_KC_SCROLL          = 0x46,    /* Scroll Lock */
		E_KC_NUMPAD7         = 0x47,
		E_KC_NUMPAD8         = 0x48,
		E_KC_NUMPAD9         = 0x49,
		E_KC_SUBTRACT        = 0x4A,    /* - on numeric keypad */
		E_KC_NUMPAD4         = 0x4B,
		E_KC_NUMPAD5         = 0x4C,
		E_KC_NUMPAD6         = 0x4D,
		E_KC_ADD             = 0x4E,   /* + on numeric keypad */
		E_KC_NUMPAD1         = 0x4F,
		E_KC_NUMPAD2         = 0x50,
		E_KC_NUMPAD3         = 0x51,
		E_KC_NUMPAD0         = 0x52,
		E_KC_DECIMAL         = 0x53,    /* . on numeric keypad */
		E_KC_OEM_102         = 0x56,    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
		E_KC_F11             = 0x57,
		E_KC_F12             = 0x58,
		E_KC_F13             = 0x64,    /*                     (NEC PC98) */
		E_KC_F14             = 0x65,    /*                     (NEC PC98) */
		E_KC_F15             = 0x66,    /*                     (NEC PC98) */
		E_KC_KANA            = 0x70,    /* (Japanese keyboard)            */
		E_KC_ABNT_C1         = 0x73,    /* /? on Brazilian keyboard */
		E_KC_CONVERT         = 0x79,    /* (Japanese keyboard)            */
		E_KC_NOCONVERT       = 0x7B,    /* (Japanese keyboard)            */
		E_KC_YEN             = 0x7D,    /* (Japanese keyboard)            */
		E_KC_ABNT_C2         = 0x7E,    /* Numpad . on Brazilian keyboard */
		E_KC_NUMPADEQUALS    = 0x8D,    /* = on numeric keypad (NEC PC98) */
		E_KC_PREVTRACK       = 0x90,    /* Previous Track (E_KC_CIRCUMFLEX on Japanese keyboard) */
		E_KC_AT              = 0x91,    /*                     (NEC PC98) */
		E_KC_COLON           = 0x92,    /*                     (NEC PC98) */
		E_KC_UNDERLINE       = 0x93,    /*                     (NEC PC98) */
		E_KC_KANJI           = 0x94,    /* (Japanese keyboard)            */
		E_KC_STOP            = 0x95,    /*                     (NEC PC98) */
		E_KC_AX              = 0x96,    /*                     (Japan AX) */
		E_KC_UNLABELED       = 0x97,    /*                        (J3100) */
		E_KC_NEXTTRACK       = 0x99,    /* Next Track */
		E_KC_NUMPADENTER     = 0x9C,    /* Enter on numeric keypad */
		E_KC_RCONTROL        = 0x9D,
		E_KC_MUTE            = 0xA0,    /* Mute */
		E_KC_CALCULATOR      = 0xA1,    /* Calculator */
		E_KC_PLAYPAUSE       = 0xA2,    /* Play / Pause */
		E_KC_MEDIASTOP       = 0xA4,    /* Media Stop */
		E_KC_VOLUMEDOWN      = 0xAE,    /* Volume - */
		E_KC_VOLUMEUP        = 0xB0,    /* Volume + */
		E_KC_WEBHOME         = 0xB2,    /* Web home */
		E_KC_NUMPADCOMMA     = 0xB3,    /* , on numeric keypad (NEC PC98) */
		E_KC_DIVIDE          = 0xB5,    /* / on numeric keypad */
		E_KC_SYSRQ           = 0xB7,
		E_KC_RMENU           = 0xB8,    /* right Alt */
		E_KC_PAUSE           = 0xC5,    /* Pause */
		E_KC_HOME            = 0xC7,    /* Home on arrow keypad */
		E_KC_UP              = 0xC8,    /* UpArrow on arrow keypad */
		E_KC_PG_UP           = 0xC9,    /* PgUp on arrow keypad */
		E_KC_LEFT            = 0xCB,    /* LeftArrow on arrow keypad */
		E_KC_RIGHT           = 0xCD,    /* RightArrow on arrow keypad */
		E_KC_END             = 0xCF,    /* End on arrow keypad */
		E_KC_DOWN            = 0xD0,    /* DownArrow on arrow keypad */
		E_KC_PG_DOWN         = 0xD1,    /* PgDn on arrow keypad */
		E_KC_INSERT          = 0xD2,    /* Insert on arrow keypad */
		E_KC_DELETE          = 0xD3,    /* Delete on arrow keypad */
		E_KC_LWIN            = 0xDB,    /* Left Windows key */
		E_KC_RWIN            = 0xDC,    /* Right Windows key */
		E_KC_APPS            = 0xDD,    /* AppMenu key */
		E_KC_POWER           = 0xDE,    /* System Power */
		E_KC_SLEEP           = 0xDF,    /* System Sleep */
		E_KC_WAKE            = 0xE3,    /* System Wake */
		E_KC_WEBSEARCH       = 0xE5,    /* Web Search */
		E_KC_WEBFAVORITES    = 0xE6,    /* Web Favorites */
		E_KC_WEBREFRESH      = 0xE7,    /* Web Refresh */
		E_KC_WEBSTOP         = 0xE8,    /* Web Stop */
		E_KC_WEBFORWARD      = 0xE9,    /* Web Forward */
		E_KC_WEBBACK         = 0xEA,    /* Web Back */
		E_KC_MYCOMPUTER      = 0xEB,    /* My Computer */
		E_KC_MAIL            = 0xEC,    /* Mail */
		E_KC_MEDIASELECT     = 0xED    /* Media Select */
	};

} } // namespace ae { namespace input {

#endif // #ifdef AE_INPUT_SYS_KEY_CODES_H
