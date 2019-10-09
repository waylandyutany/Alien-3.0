#include "StdAfx.h"

#include "export_pointers_table.h"

#include "../../base_sys/memory/allocators/incremental_allocator.h"
#include "../../debug_sys/output/outputs.h"

namespace ae { namespace resource {

C_PointersTableExport::C_PointersTableExport(char* buffer, size_t tableOffset, ae::base::C_IncrementalAllocator& allocator) :
	m_pBuffer(buffer),
	m_TableOffset(tableOffset),
	m_Allocator(allocator)
{
	m_pNumPointers = reinterpret_cast<size_t*>(m_Allocator.Alloc(sizeof(size_t)));
	*m_pNumPointers = 0;
}

void C_PointersTableExport::AddPointer(char* address)
{
	size_t offset = address - m_pBuffer;
	size_t* pOffset = reinterpret_cast<size_t*>(m_Allocator.Alloc(sizeof(size_t)));
	*pOffset = offset;
	*m_pNumPointers = *m_pNumPointers + 1;
}

void C_PointersTableExport::MakeOffsetsRelative()
{
//	DBG_OUT << "C_PointersTableExport::MakeOffsetsRelative()" << std::endl;
//	DBG_OUT << "m_pNumPointers" << *m_pNumPointers << std::endl;

	size_t* pOffsets = reinterpret_cast<size_t*>(m_pBuffer + m_TableOffset + sizeof(size_t));
	for(size_t i = 0; i < *m_pNumPointers; ++i)
	{
		size_t* offsetPointer = reinterpret_cast<size_t*>(m_pBuffer + pOffsets[i]);
//		DBG_OUT << i << " : Offset : " << pOffsets[i] << " Absolute : " << *offsetPointer << " Relative : ";
		if(*offsetPointer) *offsetPointer = *offsetPointer - reinterpret_cast<size_t>(m_pBuffer);
//		DBG_OUT << *offsetPointer << std::endl;
	}
}

} } // namespace ae { namespace resource {
