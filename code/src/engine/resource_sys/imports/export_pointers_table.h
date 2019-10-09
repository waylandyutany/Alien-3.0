#pragma once

#ifndef AE_RESOURCE_SYS_EXPORT_POINTERS_TABLE_H
#define AE_RESOURCE_SYS_EXPORT_POINTERS_TABLE_H

namespace ae { namespace base {
	class C_IncrementalAllocator;
} } // namespace ae { namespace base {

namespace ae { namespace resource {

class C_PointersTableExport
{
	C_PointersTableExport();
	NOCOPY_CLASS(C_PointersTableExport);

	char* m_pBuffer;
	size_t m_TableOffset;
	ae::base::C_IncrementalAllocator& m_Allocator;
	size_t* m_pNumPointers;

public:
	C_PointersTableExport(char* buffer, size_t tableOffset, ae::base::C_IncrementalAllocator& allocator);
	void AddPointer(char* address);
	void MakeOffsetsRelative();
};

} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_EXPORT_POINTERS_TABLE_H
