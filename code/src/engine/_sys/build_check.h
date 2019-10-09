#pragma once
#ifndef AE_BUILD_CHECK_H
#define AE_BUILD_CHECK_H

#ifdef AE_DEBUG
	#if defined(AE_DEVELOP) || defined(AE_RELEASE)
		#error Wrong configuration !
	#endif
#elif AE_DEVELOP
	#if defined(AE_DEBUG) || defined(AE_RELEASE)
		#error Wrong configuration !
	#endif
#elif AE_RELEASE
	#if defined(AE_DEBUG) || defined(AE_DEVELOP)
		#error Wrong configuration !
	#endif
#else
	#error No configuration specified !
#endif

#endif // #ifdef AE_BUILD_CHECK_H
