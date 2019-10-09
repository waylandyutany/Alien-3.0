#pragma once

#ifndef AE_RESOURCE_SYS_PACKAGE_HEADER_H
#define AE_RESOURCE_SYS_PACKAGE_HEADER_H

#include "../base_sys/hash.h"
#include "resource.h"

namespace ae { namespace resource {

	typedef char T_PackageID[4];

	struct S_Package
	{
		struct S_ResourceChunk
		{
			ae::base::T_Hash32	resourcePathHash;
			size_t				resourceOffset;
			size_t				resourceSize;
		};

		T_PackageID			packageID;	// 'pckg'
		ae::T_PlatformID	platformID;
		ae::u32				version;
		ae::u32				flags;
		ae::u32				numResourceChunks;
		S_ResourceChunk*	resourceChunks;
	};

} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_PACKAGE_HEADER_H
