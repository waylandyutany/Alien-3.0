#include "StdAfx.h"

#include "memory_seekable_input_stream.h"
#include "../../math_sys/types.h"

namespace ae { namespace base {

C_MemorySeekableInputStream::C_MemorySeekableInputStream(void* buffer, size_t size) :
	m_pStreamBuffer(static_cast<char*>(buffer)),
	m_StreamPos(m_pStreamBuffer),
	m_StreamSize(size)
{
}

C_MemorySeekableInputStream::~C_MemorySeekableInputStream()
{
	m_pStreamBuffer = 0;
	m_StreamPos = 0;
	m_StreamSize = 0;
}

size_t C_MemorySeekableInputStream::Position() const
{
	return m_StreamPos - m_pStreamBuffer;
}

bool C_MemorySeekableInputStream::Read(void* buffer, size_t size, size_t& readed)
{
	const size_t streamPos(Position());
	if(streamPos + size > m_StreamSize)
		size = m_StreamSize - streamPos;

	memcpy(buffer, m_StreamPos, size);
	readed = size;
	m_StreamPos += size;

	return true;
}

bool C_MemorySeekableInputStream::Seek(size_t position)
{
	if(position > m_StreamSize)
	{
		position = m_StreamSize;
		return false;
	}

	m_StreamPos = m_pStreamBuffer + position;
	return true;
}

size_t C_MemorySeekableInputStream::Size() const
{
	return m_StreamSize;
}

} } // namespace ae { namespace base {
