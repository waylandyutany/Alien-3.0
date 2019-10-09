#pragma once
#if !defined(AE_MENU_WIN32_H) && defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#define AE_MENU_WIN32_H

namespace ae { namespace windows_sys {  namespace win32 {

	typedef HMENU T_MenuID;
	typedef ::ae::u32 T_ControlID;
	class C_Window_Win32;

	class C_Menu_Win32
	{
		bool m_bDestroyable;
		T_MenuID m_MenuID;
		C_Menu_Win32(T_MenuID menuID) : m_bDestroyable(false), m_MenuID(menuID) {}
	public:
		C_Menu_Win32();
		~C_Menu_Win32();

		void AppendMenu(const char* sLabel, T_ControlID);
		C_Menu_Win32 AppendSubMenu(const char* sLabel);
		void AppendSeparator();

		T_ControlID TrackPopupMenu(C_Window_Win32& parent);

		operator HMENU() { return m_MenuID; }
	};

} } } // namespace ae { namespace windows_sys {  namespace win32 {


namespace ae { namespace windows_sys {
	typedef ae::windows_sys::win32::C_Menu_Win32 T_MenuImpl;
	typedef ae::windows_sys::win32::T_MenuID T_MenuID;
	typedef ae::windows_sys::win32::T_ControlID T_ControlID;
} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_MENU_WIN32_H) && defined(AE_EDITOR)
