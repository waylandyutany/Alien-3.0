#pragma once
#ifndef AE_ASSERT_H
#define AE_ASSERT_H

#include "assert_win32.h"

#if !defined(AE_DEBUG) && !defined(AE_DEVELOP)
	#ifdef AE_ASSERT
		#undef AE_ASSERT(p)
	#endif
		#define AE_ASSERT(p) (void(0))
	#endif // #if defined(AE_DEBUG) || defined(AE_DEVELOP)

#endif // #ifdef AE_ASSERT_H
