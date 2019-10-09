#pragma once

#ifndef AE_MATH_SYS_FLOAT_CONSTANTS_H
#define AE_MATH_SYS_FLOAT_CONSTANTS_H

namespace ae { namespace math {

	static const float min_float		= FLT_MIN;
	static const float max_float		= FLT_MAX;
	static const float very_small_float = 1.e-8f;
	static const float small_float		= 1.e-4f;
	static const float pi				= 3.1415926535f;
	static const float pi_inv			= 0.3183098861f;
	static const float half_pi			= pi * 0.5f;
	static const float pi2				= pi * 2.0f;
	static const float delta			= 0.00001f;

} } // namespace ae { namespace math {

#endif // #ifdef AE_MATH_SYS_FLOAT_CONSTANTS_H
