#pragma once

#ifndef AE_BASE_SYS_SEEKABLE_INPUT_STREAM_H
#define AE_BASE_SYS_SEEKABLE_INPUT_STREAM_H

#include "i_input_stream.h"

namespace ae { namespace base {

	class I_SeekableInputStream : public I_InputStream
	{
		NOCOPY_CLASS(I_SeekableInputStream);
	protected:
		I_SeekableInputStream() {};
	public:
		virtual bool Seek(size_t position) = 0;
	};

} } // namespace ae { namespace base {

#endif // #ifdef AE_BASE_SYS_SEEKABLE_INPUT_STREAM_H
