#pragma once

#ifndef AE_DEBUG_SYS_STD_OUTPUT_H
#define AE_DEBUG_SYS_STD_OUTPUT_H

#include "outputs.h"

namespace ae { namespace dbg {

class C_StdOutput : public C_Output
{
public:
	C_StdOutput() {};

	virtual void OnData(const char * s, std::streamsize n);
	virtual void OnTag(E_OutputTags, unsigned __int32);
};

} } // namespace ae { namespace dbg {

#endif // #ifdef AE_DEBUG_SYS_STD_OUTPUT_H
