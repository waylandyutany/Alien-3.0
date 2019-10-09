#include "StdAfx.h"

#include "std_output.h"

namespace ae { namespace dbg {

void C_StdOutput::OnData(const char * s, std::streamsize /*n*/)
{
	std::cout << s;
}

void C_StdOutput::OnTag(E_OutputTags /*tag*/, unsigned __int32 /*tagData*/)
{
}

} } // namespace ae { namespace dbg {
