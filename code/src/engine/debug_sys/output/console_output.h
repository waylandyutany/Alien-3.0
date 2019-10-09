#pragma once

#ifndef AE_DEBUG_SYS_CONSOLE_OUTPUT_H
#define AE_DEBUG_SYS_CONSOLE_OUTPUT_H

#include "console_output_win32.h"

#include "outputs.h"

namespace ae { namespace dbg {

class C_ConsoleOutput : public C_Output
{
	C_ConsoleOutputWin32 m_Output;

public:
	C_ConsoleOutput(const char* consoleTitle);
	~C_ConsoleOutput();

	void SetTitle(const char* title);

	virtual void OnData(const char * s, std::streamsize n);
	virtual void OnTag(E_OutputTags, unsigned __int32);
};

inline void C_ConsoleOutput::SetTitle(const char* title)
{
	m_Output.SetTitle(title);
}

} } // namespace ae { namespace dbg {

#endif // #ifdef AE_DEBUG_SYS_CONSOLE_OUTPUT_H
