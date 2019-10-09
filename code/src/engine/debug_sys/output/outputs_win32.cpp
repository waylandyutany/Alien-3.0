#include "StdAfx.h"

#include "outputs.h"

namespace ae { namespace dbg {

// ************************************************************************************************************
// ************************************************************************************************************

std::ostream& pid(std::ostream& os)
{
	DWORD processID(GetCurrentProcessId());

	doutputs.PutTag(E_OT_PID, processID);

	os << "PID[";
	os << std::uppercase << std::showbase << std::setfill('0') << std::setw(10) << std::internal << std::hex ;
	os << processID;
	os << clr_fmt;
	os << "]";

	return os;
}

std::ostream& tid(std::ostream& os)
{
	DWORD threadID(GetThreadId(GetCurrentThread()));

	doutputs.PutTag(E_OT_TID, threadID);

	os << "TID[";
	os << std::uppercase << std::showbase << std::setfill('0') << std::setw(10) << std::internal << std::hex ;
	os << threadID;
	os << clr_fmt;
	os << "]";

	return os;
}

// ************************************************************************************************************
// ************************************************************************************************************

} } // namespace ae { namespace dbg {
