#pragma once

#ifndef AE_RESOURCE_SYS_IMPORT_POINTERS_TABLE_H
#define AE_RESOURCE_SYS_IMPORT_POINTERS_TABLE_H

namespace ae { namespace resource {

class C_PointersTableImport
{
	C_PointersTableImport();
	NOCOPY_CLASS(C_PointersTableImport);

	char* m_pBuffer;
	size_t m_TableOffset;
public:
	C_PointersTableImport(char* buffer, size_t tableOffset);
	void MakePointersAbsolute();
};

} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_IMPORT_POINTERS_TABLE_H
