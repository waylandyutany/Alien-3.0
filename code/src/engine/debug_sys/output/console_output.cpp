#include "StdAfx.h"

#include "console_output.h"

namespace ae { namespace dbg {

C_ConsoleOutput::C_ConsoleOutput(const char* consoleTitle) :
	m_Output(consoleTitle)
{
}

C_ConsoleOutput::~C_ConsoleOutput()
{
}

void C_ConsoleOutput::OnData(const char * s, std::streamsize n)
{
	m_Output.OnData(s,n);
}

void C_ConsoleOutput::OnTag(E_OutputTags tag, unsigned __int32 tagData)
{
	m_Output.OnTag(tag, tagData);
}

} } // namespace ae { namespace dbg {
