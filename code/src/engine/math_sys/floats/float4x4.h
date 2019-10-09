#pragma once

#ifndef AE_MATH_SYS_FLOAT4x4_H
#define AE_MATH_SYS_FLOAT4x4_H

namespace ae { namespace math {

/*
First index is Row, second index is Collumn
| m[0][0] m[0][1] m[0][2] m[0][3] |
| m[1][0] m[1][1] m[1][2] m[1][3] |
| m[2][0] m[2][1] m[2][2] m[2][3] |
| m[3][0] m[3][1] m[3][2] m[3][3] |
*/
struct float4x4
{
	float4x4() {}

	float4x4(	float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33
			);
// *********************************************************************************************************
	float Determinant() const;
	float4x4 Inverse() const;
	float4x4 Transpose() const;
// *********************************************************************************************************
	void Translate(float x, float y, float z);
	void Translate(const float3& translation);

	// Rotation is given by radians
	void Rotate(float rx, float ry, float rz);
	void Rotate(const float3& rotation);

	void Scale(float sx, float sy, float sz);
	void Scale(const float3& scale);
	void Scale(float scale);

	// ! ! ! Rotation is given by radians ! ! !
	void TranslateRotateScale(const float3& translation, const float3& rotation, const float3& scale);
	void TranslateRotateScale(const float3& translation, const float3& rotation, float scale);

	void Identity() { *this = identity; }
	void SetColumn(ae::u32 columnIndex, const float3& column, float w)
	{
		m[0][columnIndex] = column.x;
		m[1][columnIndex] = column.y;
		m[2][columnIndex] = column.z;
		m[3][columnIndex] = w;
	}
// *********************************************************************************************************
	friend float4 operator * (const float4x4& m, const float4& v4)
	{
		float4 pRet;

		float* pSrcMat(const_cast<float*>(&m.m[0][0]));
		const float* pSrcVec(reinterpret_cast<const float*>(&v4));
		float* pDstVec(reinterpret_cast<float*>(&pRet));

		__m128 row0, row1, row2, row3, vec;
		row0 = _mm_loadu_ps(pSrcMat);
		row1 = _mm_loadu_ps(pSrcMat + 4);
		row2 = _mm_loadu_ps(pSrcMat + 8);
		row3 = _mm_loadu_ps(pSrcMat + 12);
		_MM_TRANSPOSE4_PS(row0, row1, row2, row3);

		vec = _mm_loadu_ps(pSrcVec);
		row0 = _mm_mul_ps(row0,vec);
		row1 = _mm_mul_ps(row1,vec);
		row2 = _mm_mul_ps(row2,vec);
		row3 = _mm_mul_ps(row3,vec);
		_MM_TRANSPOSE4_PS(row0, row1, row2, row3);

		row0 = _mm_add_ps(row0,row1);
		row1 = _mm_add_ps(row2,row3);
		row0 = _mm_add_ps(row0,row1);

		_mm_storeu_ps(pDstVec, row0);

		return pRet;
/*
		return float4	(
							v4.x * m.m[0][0] + v4.y * m.m[1][0] + v4.z * m.m[2][0] + v4.w * m.m[3][0],
							v4.x * m.m[0][1] + v4.y * m.m[1][1] + v4.z * m.m[2][1] + v4.w * m.m[3][1],
							v4.x * m.m[0][2] + v4.y * m.m[1][2] + v4.z * m.m[2][2] + v4.w * m.m[3][2],
							v4.x * m.m[0][3] + v4.y * m.m[1][3] + v4.z * m.m[2][3] + v4.w * m.m[3][3]
						);
*/
	}
	
	float4x4 operator* (const float4x4& Other) const
	{
		float4x4 Result;
		Result.m[0][0] = m[0][0] * Other.m[0][0] + m[0][1] * Other.m[1][0] + m[0][2] * Other.m[2][0] + m[0][3] * Other.m[3][0];
		Result.m[0][1] = m[0][0] * Other.m[0][1] + m[0][1] * Other.m[1][1] + m[0][2] * Other.m[2][1] + m[0][3] * Other.m[3][1];
		Result.m[0][2] = m[0][0] * Other.m[0][2] + m[0][1] * Other.m[1][2] + m[0][2] * Other.m[2][2] + m[0][3] * Other.m[3][2];
		Result.m[0][3] = m[0][0] * Other.m[0][3] + m[0][1] * Other.m[1][3] + m[0][2] * Other.m[2][3] + m[0][3] * Other.m[3][3];

		Result.m[1][0] = m[1][0] * Other.m[0][0] + m[1][1] * Other.m[1][0] + m[1][2] * Other.m[2][0] + m[1][3] * Other.m[3][0];
		Result.m[1][1] = m[1][0] * Other.m[0][1] + m[1][1] * Other.m[1][1] + m[1][2] * Other.m[2][1] + m[1][3] * Other.m[3][1];
		Result.m[1][2] = m[1][0] * Other.m[0][2] + m[1][1] * Other.m[1][2] + m[1][2] * Other.m[2][2] + m[1][3] * Other.m[3][2];
		Result.m[1][3] = m[1][0] * Other.m[0][3] + m[1][1] * Other.m[1][3] + m[1][2] * Other.m[2][3] + m[1][3] * Other.m[3][3];

		Result.m[2][0] = m[2][0] * Other.m[0][0] + m[2][1] * Other.m[1][0] + m[2][2] * Other.m[2][0] + m[2][3] * Other.m[3][0];
		Result.m[2][1] = m[2][0] * Other.m[0][1] + m[2][1] * Other.m[1][1] + m[2][2] * Other.m[2][1] + m[2][3] * Other.m[3][1];
		Result.m[2][2] = m[2][0] * Other.m[0][2] + m[2][1] * Other.m[1][2] + m[2][2] * Other.m[2][2] + m[2][3] * Other.m[3][2];
		Result.m[2][3] = m[2][0] * Other.m[0][3] + m[2][1] * Other.m[1][3] + m[2][2] * Other.m[2][3] + m[2][3] * Other.m[3][3];

		Result.m[3][0] = m[3][0] * Other.m[0][0] + m[3][1] * Other.m[1][0] + m[3][2] * Other.m[2][0] + m[3][3] * Other.m[3][0];
		Result.m[3][1] = m[3][0] * Other.m[0][1] + m[3][1] * Other.m[1][1] + m[3][2] * Other.m[2][1] + m[3][3] * Other.m[3][1];
		Result.m[3][2] = m[3][0] * Other.m[0][2] + m[3][1] * Other.m[1][2] + m[3][2] * Other.m[2][2] + m[3][3] * Other.m[3][2];
		Result.m[3][3] = m[3][0] * Other.m[0][3] + m[3][1] * Other.m[1][3] + m[3][2] * Other.m[2][3] + m[3][3] * Other.m[3][3];
		return Result;
	}

	float m[4][4];
	static const float4x4 identity;
};

} } // namespace ae { namespace math {

#include "float4x4.inl"

#endif // #ifdef AE_MATH_SYS_FLOAT4x4_H
