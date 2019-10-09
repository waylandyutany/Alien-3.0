#pragma once

#ifndef AE_MATH_SYS_FLOAT3x3_H
#define AE_MATH_SYS_FLOAT3x3_H

namespace ae { namespace math {

	/*
	First index is Row, second index is Column
	| m[0][0] m[0][1] m[0][2] |
	| m[1][0] m[1][1] m[1][2] |
	| m[2][0] m[2][1] m[2][2] |
	*/
	struct float3x3
	{
		float m[3][3];
	};

} } // namespace ae { namespace math {

#include "float3x3.inl"

#endif // #ifdef AE_MATH_SYS_FLOAT3x3_H
