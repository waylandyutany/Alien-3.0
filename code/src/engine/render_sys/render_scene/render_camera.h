#pragma once

#ifndef AE_RENDER_SYS_CAMERA_H
#define AE_RENDER_SYS_CAMERA_H

namespace ae { namespace render {

	class C_Camera
	{
		float fFOV, fAspect, fNear, fFar;
		ae::math::float3 rot, pos;

		D3DXVECTOR3 up, front, right;
		D3DXMATRIX projMatrix, viewMatrix;
	public:
		C_Camera();
// *****************************************************************************************************************
		void SetPos(const ae::math::float3& pos);
		void SetRot(const ae::math::float3& rot);

		ae::math::float3 GetPos()	const	{ return pos;	}
		ae::math::float3 GetRot()	const	{ return rot;	}
		ae::math::float3 GetUp()	const	{ return up;	}
		ae::math::float3 GetRight()	const	{ return right;	}
		ae::math::float3 GetDir()	const	{ return front;	}

		void GetViewMatrix(D3DXMATRIX&) const;
// *****************************************************************************************************************
		void SetFOV(float);
		float GetFOV()		const	{ return fFOV;		}
// *****************************************************************************************************************
//		atm those methods works only from render scene, since render scene always restoring it's own near/far/aspect values
		void SetNear(float);
		void SetFar(float);
		void SetAspect(float);

		float GetNear()		const	{ return fNear;		}
		float GetFar()		const	{ return fFar;		}
		float GetAspect()	const	{ return fAspect;	}
// *****************************************************************************************************************
		void GetProjMatrix(D3DXMATRIX&) const;
// *****************************************************************************************************************
		void CalculateFrustumPoints(ae::math::float3[9]) const;
		void CalculateFrustumPoints(ae::math::float3[9], float zNear, float zFar) const;
	};

} } // namespace ae { namespace render {

#endif // #ifdef AE_RENDER_SYS_CAMERA_H
