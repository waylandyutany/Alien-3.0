#include "StdAfx.h"

#include "file_output.h"

namespace ae { namespace dbg {

C_FileOutput::C_FileOutput(const char* fileName) : 
	m_Output(fileName)
{
}

void C_FileOutput::OnData(const char * s, std::streamsize n)
{
	m_Output.OnData(s,n);
}

void C_FileOutput::OnTag(E_OutputTags tag, unsigned __int32 tagData)
{
	m_Output.OnTag(tag, tagData);
}

} } // namespace ae { namespace dbg {
