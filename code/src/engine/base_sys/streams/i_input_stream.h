#pragma once

#ifndef AE_BASE_SYS_INPUT_STREAM_H
#define AE_BASE_SYS_INPUT_STREAM_H

namespace ae { namespace base {

	class I_InputStream
	{
		NOCOPY_CLASS(I_InputStream);
	protected:
		I_InputStream() {};
	public:
		virtual ~I_InputStream() {};
		virtual bool Read(void* buffer, size_t size, size_t& readed) = 0;
		virtual size_t Size() const = 0;
		virtual size_t Position() const = 0;
	};

} } // namespace ae { namespace base {

#endif // #ifdef AE_BASE_SYS_INPUT_STREAM_H
