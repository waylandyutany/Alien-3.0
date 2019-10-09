#pragma once
#ifndef AE_ASSERT_WIN32_H
#define AE_ASSERT_WIN32_H

namespace ae { namespace _sys {

void DoAssert(const char* msg, const char* file, unsigned int line);
bool IsDebuggerPresent();

namespace assert {
	inline bool IsTrue(bool b) { return b; }
} // namespace assert {

} } // namespace ae { namespace _sys {

#if defined(AE_DEBUG) || defined(AE_DEVELOP)
	#define AE_ASSERT(p) if(!ae::_sys::assert::IsTrue(p)) { if(::ae::_sys::IsDebuggerPresent())	{ __debugbreak(); }; ::ae::_sys::DoAssert(#p, __FILE__,__LINE__);}
#endif // #if defined(AE_DEBUG) || defined(AE_DEVELOP)

#define AE_CHECK(p) if(!ae::_sys::assert::IsTrue(p)) { if(::ae::_sys::IsDebuggerPresent())	{ __debugbreak(); }; ::ae::_sys::DoAssert(#p, __FILE__,__LINE__);}

#endif // #ifdef AE_ASSERT_WIN32_H
