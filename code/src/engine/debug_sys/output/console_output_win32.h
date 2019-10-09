#pragma once

#ifndef AE_DEBUG_SYS_CONSOLE_OUTPUT_WIN32_H
#define AE_DEBUG_SYS_CONSOLE_OUTPUT_WIN32_H

#include "outputs.h"

namespace ae { namespace dbg {

class C_ConsoleOutputWin32
{
	HANDLE m_hConsoleHandle;

	void ScrollUp(); // Too slow !!!
public:
	C_ConsoleOutputWin32(const char* consoleTitle);
	~C_ConsoleOutputWin32();

	void SetTitle(const char* title);

	void OnData(const char * s, std::streamsize n);
	void OnTag(E_OutputTags, unsigned __int32);
};


} } // namespace ae { namespace dbg {

#endif // #ifdef AE_DEBUG_SYS_CONSOLE_OUTPUT_WIN32_H
