#pragma once

#ifndef AE_BASE_SYS_ALLOCATOR_H
#define AE_BASE_SYS_ALLOCATOR_H


namespace ae { namespace base {

class C_DefaultAllocator
{
public:
	C_DefaultAllocator() {}

	void* Alloc(size_t size) {
		return ::malloc(size);
	}

	void Free(void* ptr) {
		::free(ptr);
	}
};

extern C_DefaultAllocator theDefaultAllocator;

} } // namespace ae { namespace base {

#endif // #ifdef AE_BASE_SYS_ALLOCATOR_H
