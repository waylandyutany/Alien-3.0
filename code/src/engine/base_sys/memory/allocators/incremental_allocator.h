#pragma once

#ifndef AE_BASE_SYS_INCREMENTAL_ALLOCATOR_H
#define AE_BASE_SYS_INCREMENTAL_ALLOCATOR_H

namespace ae { namespace base {

class C_IncrementalAllocator
{
	char *m_pBegin, *m_pEnd, *m_pPos;
public:
	C_IncrementalAllocator();
	~C_IncrementalAllocator();

	void Create(char* buff, size_t size);

	char* Alloc(size_t size);
	char* Alloc(size_t size, size_t alignment);
	char* Alloc(size_t size, bool zero);
	char* Alloc(size_t size, size_t alignment, bool zero);

	void Free(void*) { };

	void Reset(bool zero);
	void ResetToPointer(char* p, bool zero);

	size_t GetUsage() const;
	size_t GetRelativeUsage(void*) const;
	char* GetTopPointer() const;
	char* GetBottomPointer() const;
};

} } // namespace ae { namespace base {

#include "incremental_allocator.inl"

#endif // #ifdef AE_BASE_SYS_INCREMENTAL_ALLOCATOR_H
