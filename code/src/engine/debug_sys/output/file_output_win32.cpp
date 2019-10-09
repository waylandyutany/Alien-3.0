#include "StdAfx.h"

#include "file_output_win32.h"

namespace ae { namespace dbg {

C_FileOutputWin32::C_FileOutputWin32(const char* fileName)
{
	m_hFile = CreateFileA(fileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

C_FileOutputWin32::~C_FileOutputWin32()
{
	CloseHandle(m_hFile);
}

void C_FileOutputWin32::OnData(const char * s, std::streamsize /*n*/)
{
	DWORD written;
	static const char carriageReturn = '\r';
	if(*s == '\n')
		WriteFile(m_hFile, &carriageReturn, 1, &written, NULL);

	size_t toWrite = strlen(s);
	WriteFile(m_hFile, s, (DWORD)toWrite, &written, NULL);
}

void C_FileOutputWin32::OnTag(E_OutputTags tag, unsigned __int32 /*tagData*/)
{
	if(tag == E_OT_EOL) // when new line occured we flush buffer to file
		FlushFileBuffers(m_hFile);
}

} } // namespace ae { namespace dbg {
