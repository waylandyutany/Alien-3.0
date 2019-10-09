#include "StdAfx.h"

#include "file_input_stream.h"

namespace ae { namespace base {

C_FileInputStream::C_FileInputStream(const char* fileName) : m_pFile(0)
{
	if(fopen_s(&m_pFile, fileName,"rb") != 0)
		m_pFile = 0;

	fseek (m_pFile, 0, SEEK_END);
	m_FileSize = ftell(m_pFile);
	fseek (m_pFile, 0, SEEK_SET);
}

C_FileInputStream::~C_FileInputStream()
{
	fclose(m_pFile);
}

bool C_FileInputStream::Read(void* buffer, size_t size, size_t& readed)
{
	readed = fread(buffer, 1, size, m_pFile);
	return true;
}

size_t C_FileInputStream::Position() const
{
	return ftell(m_pFile);
}

size_t C_FileInputStream::Size() const
{
	return m_FileSize;
}

} } // namespace ae { namespace base {
