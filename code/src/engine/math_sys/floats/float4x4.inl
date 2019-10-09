#include "float_constants.h"

namespace ae { namespace math {

	inline float4x4::float4x4(	float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33
		)
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;
		m[0][3] = m03;

		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;
		m[1][3] = m13;

		m[2][0] = m20;
		m[2][1] = m21;
		m[2][2] = m22;
		m[2][3] = m23;

		m[3][0] = m30;
		m[3][1] = m31;
		m[3][2] = m32;
		m[3][3] = m33;
	}

	//	float m[4][4];
	inline float float4x4::Determinant() const
	{
		return	m[0][0] * (
			m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
			m[2][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) +
			m[3][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2])
			) -
			m[1][0] * (
			m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
			m[2][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
			m[3][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2])
			) +
			m[2][0] * (
			m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) -
			m[1][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
			m[3][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
			) -
			m[3][0] * (
			m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) -
			m[1][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2]) +
			m[2][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
			);
	}

	inline float4x4 float4x4::Inverse() const
	{
		float4x4 ret;
		const float	d = Determinant();
		if(d == 0.0f) return float4x4::identity;
		const float	rd = 1.0f / d;

		ret.m[0][0] = rd * (
			m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
			m[2][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) +
			m[3][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2])
			);
		ret.m[0][1] = -rd * (
			m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
			m[2][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
			m[3][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2])
			);
		ret.m[0][2] = rd * (
			m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) -
			m[1][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
			m[3][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
			);
		ret.m[0][3] = -rd * (
			m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) -
			m[1][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2]) +
			m[2][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
			);

		ret.m[1][0] = -rd * (
			m[1][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
			m[2][0] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) +
			m[3][0] * (m[1][2] * m[2][3] - m[1][3] * m[2][2])
			);
		ret.m[1][1] = rd * (
			m[0][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
			m[2][0] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
			m[3][0] * (m[0][2] * m[2][3] - m[0][3] * m[2][2])
			);
		ret.m[1][2] = -rd * (
			m[0][0] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) -
			m[1][0] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
			m[3][0] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
			);
		ret.m[1][3] = rd * (
			m[0][0] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) -
			m[1][0] * (m[0][2] * m[2][3] - m[0][3] * m[2][2]) +
			m[2][0] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
			);

		ret.m[2][0] = rd * (
			m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) -
			m[2][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) +
			m[3][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1])
			);
		ret.m[2][1] = -rd * (
			m[0][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) -
			m[2][0] * (m[0][1] * m[3][3] - m[0][3] * m[3][1]) +
			m[3][0] * (m[0][1] * m[2][3] - m[0][3] * m[2][1])
			);
		ret.m[2][2] = rd * (
			m[0][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) -
			m[1][0] * (m[0][1] * m[3][3] - m[0][3] * m[3][1]) +
			m[3][0] * (m[0][1] * m[1][3] - m[0][3] * m[1][1])
			);
		ret.m[2][3] = -rd * (
			m[0][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]) -
			m[1][0] * (m[0][1] * m[2][3] - m[0][3] * m[2][1]) +
			m[2][0] * (m[0][1] * m[1][3] - m[0][3] * m[1][1])
			);

		ret.m[3][0] = -rd * (
			m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) -
			m[2][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) +
			m[3][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
			);
		ret.m[3][1] = rd * (
			m[0][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) -
			m[2][0] * (m[0][1] * m[3][2] - m[0][2] * m[3][1]) +
			m[3][0] * (m[0][1] * m[2][2] - m[0][2] * m[2][1])
			);
		ret.m[3][2] = -rd * (
			m[0][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) -
			m[1][0] * (m[0][1] * m[3][2] - m[0][2] * m[3][1]) +
			m[3][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1])
			);
		ret.m[3][3] = rd * (	
			m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
			m[1][0] * (m[0][1] * m[2][2] - m[0][2] * m[2][1]) +
			m[2][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1])
			);

		return ret;
	}

	inline float4x4 float4x4::Transpose() const
	{
		return float4x4	(
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]
		);
	}

	// *********************************************************************************************************
	inline void float4x4::Translate(float x, float y, float z)
	{
		m[3][0] = x;
		m[3][1] = y;
		m[3][2] = z;
	}

	inline void float4x4::Translate(const float3& translation) {
		Translate(translation.x, translation.y, translation.z);
	}

	inline void float4x4::Rotate(float rx, float ry, float rz)
	{
		const float	SR = sinf(rx);
		const float	SP = sinf(ry);
		const float	SY = sinf(rz);
		const float	CR = cosf(rx);
		const float	CP = cosf(ry);
		const float	CY = cosf(rz);

		m[0][0]	= CP * CY;
		m[0][1]	= CP * SY;
		m[0][2]	= SP;
		m[0][3]	= 0.f;

		m[1][0]	= SR * SP * CY - CR * SY;
		m[1][1]	= SR * SP * SY + CR * CY;
		m[1][2]	= - SR * CP;
		m[1][3]	= 0.f;

		m[2][0]	= -( CR * SP * CY + SR * SY );
		m[2][1]	= CY * SR - CR * SP * SY;
		m[2][2]	= CR * CP;
		m[2][3]	= 0.f;

		m[3][0]	= 0.f;
		m[3][1]	= 0.f;
		m[3][2]	= 0.f;
		m[3][3]	= 1.f;
	}

	inline void float4x4::Rotate(const float3& rotation) {
		Rotate(rotation.x, rotation.y, rotation.z);
	}

	inline void float4x4::Scale(float sx, float sy, float sz)
	{
		m[0][0] = sx;
		m[1][1] = sy;
		m[2][2] = sz;
	}

	inline void float4x4::Scale(const float3& scale) {
		Scale(scale.x, scale.y, scale.z);
	}

	inline void float4x4::Scale(float scale) {
		Scale(scale, scale, scale);
	}

	inline void float4x4::TranslateRotateScale(const float3& translation, const float3& rotation, const float3& scale)
	{
		AE_ASSERT(-pi2 <= rotation.x && rotation.x <= pi2 && "Rotation must be in radians !!!");
		AE_ASSERT(-pi2 <= rotation.y && rotation.y <= pi2 && "Rotation must be in radians !!!");
		AE_ASSERT(-pi2 <= rotation.z && rotation.z <= pi2 && "Rotation must be in radians !!!");

		const float	SR = sinf(rotation.x);
		const float	SP = sinf(rotation.y);
		const float	SY = sinf(rotation.z);
		const float	CR = cosf(rotation.x);
		const float	CP = cosf(rotation.y);
		const float	CY = cosf(rotation.z);

		m[0][0]	= (CP * CY) * scale.x;
		m[0][1]	= (CP * SY) * scale.x;
		m[0][2]	= (SP) * scale.x;
		m[0][3]	= 0.f;

		m[1][0]	= (SR * SP * CY - CR * SY) * scale.y;
		m[1][1]	= (SR * SP * SY + CR * CY) * scale.y;
		m[1][2]	= (- SR * CP) * scale.y;
		m[1][3]	= 0.f;

		m[2][0]	= ( -( CR * SP * CY + SR * SY ) ) * scale.z;
		m[2][1]	= (CY * SR - CR * SP * SY) * scale.z;
		m[2][2]	= (CR * CP) * scale.z;
		m[2][3]	= 0.f;

		m[3][0]	= translation.x;
		m[3][1]	= translation.y;
		m[3][2]	= translation.z;
		m[3][3]	= 1.f;

	}

	inline void float4x4::TranslateRotateScale(const float3& translation, const float3& rotation, float scale)
	{
		const float3 scale3(scale,scale,scale);
		TranslateRotateScale(translation, rotation, scale3);
	}

} } // namespace ae { namespace math {
