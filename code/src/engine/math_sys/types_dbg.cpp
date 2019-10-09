#include "StdAfx.h"

#include "types_dbg.h"

namespace ae { namespace math {

std::ostream& operator << (std::ostream& os, const float4x4& m)
{
	os << "| " << m.m[0][0] << " | " << m.m[0][1] << " | " << m.m[0][2] << " | " << m.m[0][3] << " |" << std::endl;
	os << "| " << m.m[1][0] << " | " << m.m[1][1] << " | " << m.m[1][2] << " | " << m.m[1][3] << " |" << std::endl;
	os << "| " << m.m[2][0] << " | " << m.m[2][1] << " | " << m.m[2][2] << " | " << m.m[2][3] << " |" << std::endl;
	os << "| " << m.m[3][0] << " | " << m.m[3][1] << " | " << m.m[3][2] << " | " << m.m[3][3] << " |" << std::endl;

	return os;
}

} } // namespace ae { namespace math {
