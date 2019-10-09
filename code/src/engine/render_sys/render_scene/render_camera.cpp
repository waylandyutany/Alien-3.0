#include "StdAfx.h"

#include "render_camera.h"

namespace ae { namespace render {

	C_Camera::C_Camera() : 
		fFOV(((float)D3DX_PI / 4.0f)), fAspect(800.0f/600.0f), 
		fNear(.1f), fFar(1000.0f)
	{
		SetPos(ae::math::float3(0,0,0));
		SetRot(ae::math::float3(0,0,0));
	}

	void C_Camera::SetPos(const ae::math::float3& pos)
	{
		this->pos = pos;

		D3DXVECTOR3 lookPos(pos);
		D3DXVECTOR3 lookAt = lookPos + front;
		D3DXMatrixLookAtLH(&viewMatrix, &lookPos, &lookAt, &up);
	}

	void C_Camera::SetRot(const ae::math::float3& rot)
	{
		this->rot = rot;

		D3DXMATRIX rotationMatrix;
		up		= D3DXVECTOR3(0,1,0);
		front	= D3DXVECTOR3(0,0,1);
		right	= D3DXVECTOR3(1,0,0);

		// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
		// Create the rotation matrix from the yaw, pitch, and roll values.
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rot.y * 0.0174532925f, rot.x * 0.0174532925f, rot.z * 0.0174532925f);

		// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
		D3DXVec3TransformCoord(&up,		&up,	&rotationMatrix);
		D3DXVec3TransformCoord(&front,	&front, &rotationMatrix);
		D3DXVec3TransformCoord(&right,	&right, &rotationMatrix);

		D3DXVECTOR3 lookPos(pos);
		D3DXVECTOR3 lookAt = lookPos + front;
		D3DXMatrixLookAtLH(&viewMatrix, &lookPos, &lookAt, &up);
	}

	void C_Camera::GetViewMatrix(D3DXMATRIX& m) const
	{
		m = viewMatrix;
	}

	// *****************************************************************************************************************

	void C_Camera::SetNear(float fNear) {
		//todo call D3DXMatrixPerspectiveFovLH from GetProjMatrix only when it changed
		this->fNear = fNear;
		D3DXMatrixPerspectiveFovLH(&projMatrix, fFOV, fAspect, fNear, fFar);
	}

	void C_Camera::SetFar(float fFar) {
		//todo call D3DXMatrixPerspectiveFovLH from GetProjMatrix only when it changed
		this->fFar = fFar;
		D3DXMatrixPerspectiveFovLH(&projMatrix, fFOV, fAspect, fNear, fFar);
	}

	void C_Camera::SetFOV(float fFOV) {
		//todo call D3DXMatrixPerspectiveFovLH from GetProjMatrix only when it changed
		this->fFOV = fFOV;
		D3DXMatrixPerspectiveFovLH(&projMatrix, fFOV, fAspect, fNear, fFar);
	}

	void C_Camera::SetAspect(float fAspect)	{
		//todo call D3DXMatrixPerspectiveFovLH from GetProjMatrix only when it changed
		this->fAspect = fAspect;
		D3DXMatrixPerspectiveFovLH(&projMatrix, fFOV, fAspect, fNear, fFar);
	}

	void C_Camera::GetProjMatrix(D3DXMATRIX& m) const
	{
		m = projMatrix;
	}

// *****************************************************************************************************************

	void C_Camera::CalculateFrustumPoints(ae::math::float3 pts[9] , float zNear, float zFar) const
	{
		const float tgHalfFovX = tan(GetFOV() / 2), tgHalfFovY = tgHalfFovX / GetAspect();

		pts[0].x = pos.x + front.x * zNear - right.x * zNear * tgHalfFovX - up.x * zNear * tgHalfFovY;
		pts[0].y = pos.y + front.y * zNear - right.y * zNear * tgHalfFovX - up.y * zNear * tgHalfFovY;
		pts[0].z = pos.z + front.z * zNear - right.z * zNear * tgHalfFovX - up.z * zNear * tgHalfFovY;

		pts[1].x = pos.x + front.x * zNear + right.x * zNear * tgHalfFovX - up.x * zNear * tgHalfFovY;
		pts[1].y = pos.y + front.y * zNear + right.y * zNear * tgHalfFovX - up.y * zNear * tgHalfFovY;
		pts[1].z = pos.z + front.z * zNear + right.z * zNear * tgHalfFovX - up.z * zNear * tgHalfFovY;

		pts[2].x = pos.x + front.x * zNear + right.x * zNear * tgHalfFovX + up.x * zNear * tgHalfFovY;
		pts[2].y = pos.y + front.y * zNear + right.y * zNear * tgHalfFovX + up.y * zNear * tgHalfFovY;
		pts[2].z = pos.z + front.z * zNear + right.z * zNear * tgHalfFovX + up.z * zNear * tgHalfFovY;

		pts[3].x = pos.x + front.x * zNear - right.x * zNear * tgHalfFovX + up.x * zNear * tgHalfFovY;
		pts[3].y = pos.y + front.y * zNear - right.y * zNear * tgHalfFovX + up.y * zNear * tgHalfFovY;
		pts[3].z = pos.z + front.z * zNear - right.z * zNear * tgHalfFovX + up.z * zNear * tgHalfFovY;


		pts[4].x = pos.x + front.x * zFar - right.x * zFar * tgHalfFovX - up.x * zFar * tgHalfFovY;
		pts[4].y = pos.y + front.y * zFar - right.y * zFar * tgHalfFovX - up.y * zFar * tgHalfFovY;
		pts[4].z = pos.z + front.z * zFar - right.z * zFar * tgHalfFovX - up.z * zFar * tgHalfFovY;

		pts[5].x = pos.x + front.x * zFar + right.x * zFar * tgHalfFovX - up.x * zFar * tgHalfFovY;
		pts[5].y = pos.y + front.y * zFar + right.y * zFar * tgHalfFovX - up.y * zFar * tgHalfFovY;
		pts[5].z = pos.z + front.z * zFar + right.z * zFar * tgHalfFovX - up.z * zFar * tgHalfFovY;

		pts[6].x = pos.x + front.x * zFar + right.x * zFar * tgHalfFovX + up.x * zFar * tgHalfFovY;
		pts[6].y = pos.y + front.y * zFar + right.y * zFar * tgHalfFovX + up.y * zFar * tgHalfFovY;
		pts[6].z = pos.z + front.z * zFar + right.z * zFar * tgHalfFovX + up.z * zFar * tgHalfFovY;

		pts[7].x = pos.x + front.x * zFar - right.x * zFar * tgHalfFovX + up.x * zFar * tgHalfFovY;
		pts[7].y = pos.y + front.y * zFar - right.y * zFar * tgHalfFovX + up.y * zFar * tgHalfFovY;
		pts[7].z = pos.z + front.z * zFar - right.z * zFar * tgHalfFovX + up.z * zFar * tgHalfFovY;

		pts[8] = pos;
	}

	void C_Camera::CalculateFrustumPoints(ae::math::float3 pts[9]) const
	{
		CalculateFrustumPoints(pts, GetNear(), GetFar());
	}

} } // namespace ae { namespace render {
