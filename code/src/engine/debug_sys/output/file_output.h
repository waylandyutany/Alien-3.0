#pragma once

#ifndef AE_DEBUG_SYS_FILE_OUTPUT_H
#define AE_DEBUG_SYS_FILE_OUTPUT_H

#include "file_output_win32.h"

#include "outputs.h"

namespace ae { namespace dbg {

class C_FileOutput : public C_Output
{
	C_FileOutputWin32 m_Output;
public:
	C_FileOutput(const char* fileName);

	virtual void OnData(const char * s, std::streamsize n);
	virtual void OnTag(E_OutputTags, unsigned __int32);
};


} } // namespace ae { namespace dbg {

#endif // #ifdef AE_DEBUG_SYS_FILE_OUTPUT_H
