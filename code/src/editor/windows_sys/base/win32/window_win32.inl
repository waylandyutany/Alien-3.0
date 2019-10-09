namespace ae { namespace windows_sys {  namespace win32 {

inline void C_Window_Win32::SetMenu(C_Menu_Win32& menu)
{
	::SetMenu(m_hWnd, menu);
}

inline void C_Window_Win32::RemoveMenu()
{
	::SetMenu(m_hWnd, 0);
}

inline void C_Window_Win32::SetTimer(ae::u32 timerID, ae::u32 timerMiliseconds)
{
	::SetTimer(m_hWnd, timerID, timerMiliseconds, 0);
}

inline void C_Window_Win32::DelTimer(ae::u32 timerID)
{
	::KillTimer(m_hWnd, timerID);
}

} } } // namespace ae { namespace windows_sys {  namespace win32 {


