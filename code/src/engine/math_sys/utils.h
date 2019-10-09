#pragma once

#ifndef AE_MATH_SYS_UTILS_H
#define AE_MATH_SYS_UTILS_H

namespace ae { namespace math {

float CalcBoundingSphere(const ae::math::float3* pts, ae::u32 numPts, ae::math::float3& center);
float CalcMinBoundingSphere(const ae::math::float3* pts, ae::u32 numPts, ae::math::float3& center);
void CalcOrthonormalBasis(const ae::math::float3 &dir, ae::math::float3 &right, ae::math::float3 &up);
void CalcOrthoProjection(ae::math::float4x4& m, float l,float r,float t,float b,float zn,float zf);

} } // namespace ae { namespace math {

#include "utils.inl"

#endif // #ifdef AE_MATH_SYS_UTILS_H
