#include "StdAfx.h"

#include "console_output_win32.h"

namespace ae { namespace dbg {

C_ConsoleOutputWin32::C_ConsoleOutputWin32(const char* consoleTitle)
{
	if(!AllocConsole())
		AttachConsole(ATTACH_PARENT_PROCESS);

	SetConsoleTitleA(consoleTitle);

	m_hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD Size;
	Size.X = 160;
	Size.Y = 8000;
	SetConsoleScreenBufferSize( m_hConsoleHandle, Size );
}

C_ConsoleOutputWin32::~C_ConsoleOutputWin32()
{
	FreeConsole();
}

void C_ConsoleOutputWin32::OnData(const char * s, std::streamsize)
{
	WriteFile(m_hConsoleHandle, s, (DWORD)strlen(s), NULL, NULL);

/*  Too slow !!! especialy ScrollUp call !!!
	FlushFileBuffers(m_hConsoleHandle);
	ScrollUp();
*/
}

void C_ConsoleOutputWin32::OnTag(E_OutputTags tag, unsigned __int32)
{
	switch(tag)
	{
	case E_OT_Beep:
		MessageBeep((UINT)-1);
		break;
	case E_OT_Error:
		SetConsoleTextAttribute(m_hConsoleHandle, FOREGROUND_RED);
		break;
	case E_OT_Critical:
		SetConsoleTextAttribute(m_hConsoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case E_OT_Warning:
		SetConsoleTextAttribute(m_hConsoleHandle, FOREGROUND_BLUE | FOREGROUND_GREEN);
		break;
	case E_OT_Info:
		SetConsoleTextAttribute(m_hConsoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case E_OT_EOL:
		SetConsoleTextAttribute(m_hConsoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	}
}

void C_ConsoleOutputWin32::SetTitle(const char* title)
{
	SetConsoleTitleA(title);
}


void C_ConsoleOutputWin32::ScrollUp() // Too slow !!!
{
	CONSOLE_SCREEN_BUFFER_INFO	csbi; 
	SMALL_RECT					sr; 

	if( GetConsoleScreenBufferInfo( m_hConsoleHandle, &csbi) && csbi.srWindow.Top > 0 ) 
	{ 
		sr.Top = -1; sr.Bottom = -1; // Moving with top and bottom one level up
		sr.Left = 0; sr.Right = 0; // Keep side positions unchanged

		SetConsoleWindowInfo( m_hConsoleHandle, false, &sr );
	}
}

} } // namespace ae { namespace dbg {
