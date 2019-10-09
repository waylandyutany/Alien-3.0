#pragma once

#ifndef AE_DEBUG_SYS_FILE_OUTPUT_WIN32_H
#define AE_DEBUG_SYS_FILE_OUTPUT_WIN32_H

#include "outputs.h"

namespace ae { namespace dbg {

class C_FileOutputWin32
{
	HANDLE m_hFile;
public:
	C_FileOutputWin32(const char* fileName);
	~C_FileOutputWin32();

	virtual void OnData(const char * s, std::streamsize n);
	virtual void OnTag(E_OutputTags, unsigned __int32);
};


} } // namespace ae { namespace dbg {

#endif // #ifdef AE_DEBUG_SYS_FILE_OUTPUT_WIN32_H
