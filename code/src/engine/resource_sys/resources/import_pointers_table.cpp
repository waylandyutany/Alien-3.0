#include "StdAfx.h"

#include "import_pointers_table.h"

#include "../../debug_sys/output/outputs.h"

namespace ae { namespace resource {

C_PointersTableImport::C_PointersTableImport(char* buffer, size_t tableOffset) :
	m_pBuffer(buffer),
	m_TableOffset(tableOffset)
{
}

void C_PointersTableImport::MakePointersAbsolute()
{
//	DBG_OUT << "C_PointersTableImport::MakeOffsetsAbsolute()" << std::endl;
	size_t* pNumOffsets = reinterpret_cast<size_t*>(m_pBuffer + m_TableOffset);
	size_t* pOffsets = reinterpret_cast<size_t*>(m_pBuffer + m_TableOffset + sizeof(size_t));
//	DBG_OUT << "m_pNumPointers" << *pNumOffsets << std::endl;

	for(size_t i = 0; i < *pNumOffsets; ++i)
	{
		size_t* offsetPointer = reinterpret_cast<size_t*>(m_pBuffer + pOffsets[i]);
//		DBG_OUT << i << " : Offset : " << pOffsets[i] << " Relative : " << *offsetPointer << " Absolute : ";
		if(*offsetPointer) *offsetPointer = *offsetPointer + reinterpret_cast<size_t>(m_pBuffer);
//		DBG_OUT << *offsetPointer << std::endl;
	}
}

} } // namespace ae { namespace resource {
