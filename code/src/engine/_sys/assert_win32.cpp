#include "StdAfx.h"

#include "assert.h"

namespace ae { namespace _sys {

void DoAssert(const char* msg, const char* file, unsigned int line)
{
	std::stringstream stream;
	stream << "Assertion failed !" << std::endl << "[" << msg << "]" << std::endl << "\"" << file << "\" [" << line << "]" << std::endl;
	MessageBoxA( NULL, stream.str().c_str(), "Assertion failed !", MB_OK | MB_ICONERROR );
}

bool IsDebuggerPresent()
{
	return ::IsDebuggerPresent() ? true : false;
}

} } // namespace ae { namespace _sys {

