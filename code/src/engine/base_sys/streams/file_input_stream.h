#pragma once

#ifndef AE_BASE_SYS_FILE_INPUT_STREAM_H
#define AE_BASE_SYS_FILE_INPUT_STREAM_H

#include "i_input_stream.h"

namespace ae { namespace base {

	class C_FileInputStream : public I_InputStream
	{
		std::FILE *m_pFile;
		size_t m_FileSize;
		NOCOPY_CLASS(C_FileInputStream);
	public:
		C_FileInputStream(const char* fileName);
		~C_FileInputStream();
		virtual bool Read(void* buffer, size_t size, size_t& readed);
		virtual size_t Position() const;
		virtual size_t Size() const;
	};

} } // namespace ae { namespace base {

#endif // #ifdef AE_BASE_SYS_FILE_INPUT_STREAM_H
