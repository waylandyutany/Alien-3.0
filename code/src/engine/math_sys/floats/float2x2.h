#pragma once

#ifndef AE_MATH_SYS_FLOAT2x2_H
#define AE_MATH_SYS_FLOAT2x2_H

namespace ae { namespace math {

	/*
	First index is Row, second index is Column
	| m[0][0] m[0][1] |
	| m[1][0] m[1][1] |
	*/

	struct float2x2
	{
		float m[2][2];
	};

} } // namespace ae { namespace math {

#include "float2x2.inl"

#endif // #ifdef AE_MATH_SYS_FLOAT2x2_H
