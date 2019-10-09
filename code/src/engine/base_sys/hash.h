#pragma once

#ifndef AE_BASE_SYS_HASH_H
#define AE_BASE_SYS_HASH_H

namespace ae { namespace base {

typedef ae::u32 T_Hash32;
typedef ae::u64 T_Hash64;

inline T_Hash32 CalculateHash32(const char* str);	///< Calculate case sensitive 32 bit hash from string
inline T_Hash32 CalculateHashI32(const char* str);	///< Calculate no case sensitive 32 bit hash from string
inline T_Hash32 CalculateHash32(const void* buff, size_t size);	///< Calculate 32 bit hash from memory buffer

inline T_Hash64 CalculateHash64(const void* buff, size_t size);	///< Calculate 64 bit hash from memory buffer
inline T_Hash64 CalculateHashI64(const char* str);	///< Calculate no case sensitive 64 bit hash from string
inline T_Hash64 CalculateHash64(const char* str);	///< Calculate case sensitive 64 bit hash from string

struct S_HashID
{
	const char* id;
	T_Hash32 idHash;
	bool Is(const char* sid) const { return !_stricmp(id, sid) ? true : false; }
};

} } // namespace ae { namespace base {

#include "hash.inl"

#endif // #ifdef AE_BASE_SYS_HASH_H
