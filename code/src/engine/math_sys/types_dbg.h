#pragma once

#ifndef AE_BASE_SYS_TYPES_DBG_H
#define AE_BASE_SYS_TYPES_DBG_H

#include "types.h"

#include "../debug_sys/output/outputs.h"

namespace ae { namespace math {

template<typename T> inline std::ostream& operator << (std::ostream& os, const vector2<T>& v2)
{ os << v2.x << ", " << v2.y; return os; }

template<typename T> inline std::ostream& operator << (std::ostream& os, const vector3<T>& v3)
{ 
	os << ae::dbg::flt_fmt(2) << v3.x << ",";
	os << ae::dbg::flt_fmt(2) << v3.y << ",";
	os << ae::dbg::flt_fmt(2) << v3.z << ",";
	return os;
}

template<typename T> inline std::ostream& operator << (std::ostream& os, const vector4<T>& v4)
{ 
	os << ae::dbg::flt_fmt(2) << v4.x << ",";
	os << ae::dbg::flt_fmt(2) << v4.y << ",";
	os << ae::dbg::flt_fmt(2) << v4.z << ",";
	os << ae::dbg::flt_fmt(2) << v4.w;
	return os;
}

template<typename T> inline std::ostream& operator << (std::ostream& os, const box3d_t<T>& b)
{ os << "[" << b.min << "]  [" << b.max << "]"; return os; }

std::ostream& operator << (std::ostream& os, const float4x4& m);

} } // namespace ae { namespace math {

#endif // #ifdef AE_BASE_SYS_TYPES_DBG_H
