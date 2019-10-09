#pragma once

#ifndef AE_BASE_SYS_REUSABLE_BUFFERS_H
#define AE_BASE_SYS_REUSABLE_BUFFERS_H

namespace ae { namespace base {

	class C_ReusableBuffers
	{
		C_ReusableBuffers();
		NOCOPY_CLASS(C_ReusableBuffers);

		typedef std::list<void*> T_Buffers;
		T_Buffers m_UnacquiredBuffers;
		const ae::u32 m_BufferSize;
		const ae::u32 m_MaxBuffers;
	public:
		C_ReusableBuffers(ae::u32 bufferSize, ae::u32 maxBuffers);
		~C_ReusableBuffers();

		void AddBuffer(void*, ae::u32 bufferSize);
		void* AcquireBuffer(ae::u32 bufferSize);
		void UnacquireBuffer(void*);
	};

} } // namespace ae { namespace base {

#include "reusable_buffers.inl"

#endif // #ifdef AE_BASE_SYS_REUSABLE_BUFFERS_H
