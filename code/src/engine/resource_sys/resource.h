#pragma once

#ifndef AE_RESOURCE_SYS_RESOURCE_HEADER_H
#define AE_RESOURCE_SYS_RESOURCE_HEADER_H

#include "../base_sys/hash.h"
#include "resource_types.h"

namespace ae { namespace resource {

	typedef ae::u32 T_ResourceState;
	static const T_ResourceState resourceActivated = 1; // Resource has been activated with ResourceHandler->Activate method
	static const T_ResourceState resourceResolved = 2; // Resource has resolved it's pointers to absolute values

	struct S_Resource
	{
		size_t				totalSize;
		size_t				headerSize;
		size_t				offsetPointersTable;
		ae::base::S_HashID	hid;
		T_ResourceState		state;
		T_ResourceType		resourceType;

		inline S_Resource* Next() { return (S_Resource*)((char*)this + totalSize); }
		inline bool IsActivated() { return (state & resourceActivated) ? true : false; }
		inline bool IsResolved() { return (state & resourceResolved) ? true : false; }
		template<typename T_Type>

		T_Type* GetData() { return reinterpret_cast<T_Type*>((char*)this + headerSize); }
		size_t GetDataSize() const { return offsetPointersTable - headerSize; }
	};

} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_RESOURCE_HEADER_H
