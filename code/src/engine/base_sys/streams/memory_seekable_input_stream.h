#pragma once

#ifndef AE_BASE_SYS_MEMORY_SEEKABLE_INPUT_STREAM_H
#define AE_BASE_SYS_MEMORY_SEEKABLE_INPUT_STREAM_H

#include "i_seekable_input_stream.h"

namespace ae { namespace base {

	class C_MemorySeekableInputStream : public I_SeekableInputStream
	{
		C_MemorySeekableInputStream();
		NOCOPY_CLASS(C_MemorySeekableInputStream);

		char* m_pStreamBuffer;
		char* m_StreamPos;
		size_t m_StreamSize;
	public:
		C_MemorySeekableInputStream(void* buffer, size_t size);
		~C_MemorySeekableInputStream();
		virtual bool Read(void* buffer, size_t size, size_t& readed);
		virtual size_t Position() const;
		virtual bool Seek(size_t position);
		virtual size_t Size() const;
	};

} } // namespace ae { namespace base {

#endif // #ifdef AE_BASE_SYS_MEMORY_SEEKABLE_INPUT_STREAM_H
